# Contributing

Building takes an Apple Silicon Mac and mise.
Debugging on the device is in the [README](README.md#debugging).

## Toolchain

A fresh clone needs mise and nothing else;
after `mise run setup`, `clion .` opens the project.
`mise run test` alone also needs the system `cc`, unpinned.

Only `.idea/misc.xml` and `.idea/debug-profiles.xml` are tracked,
so a first `clion .` finds the compilation database and the debug profile,
and the rest of `.idea/` stays ignored.

SDK, toolchain and build output stay in `.ufbt/` and `dist/`.
Run ufbt through mise, from the repository root:

```shell
mise exec -- ufbt        # build
mise exec -- ufbt lint   # check the sources against Flipper's format
```

Never run `ufbt vscode_dist`:
it overwrites `.editorconfig` and `.gitignore`.

Project tasks are files in `.mise/tasks/`:

- `setup` deploys the pinned SDK and builds once,
  under a sandbox that denies writes outside the project and temp.
  It runs on macOS only.
- `build` builds the app.
- `test` builds the suites under `test/` with the system `cc` and runs them on the Mac,
  one `ok` line per test.
- `launch` builds it, uploads it to the device and starts it.
  `ufbt flash` is not the same thing: it rewrites the device's firmware.
- `debug` opens GDB on the device in the terminal.
- `check-workflow` runs the jobs of `.github/workflows/ci.yml` through `act`,
  contained, against colima's Docker socket unless `DOCKER_HOST` says otherwise.
- `release` cuts a release from `develop` and pushes it; see [Release](#release).

To bump the SDK,
edit `FLIPPER_SDK_VERSION` in `mise.toml` and run `mise run setup`.

To bump a tool,
remove `[tool_config]` from `mise.toml`,
since `locked = true` refuses a graph that does not exist yet.
Change the pin, then restore the section once the new graph is locked:

```shell
mise install uv python@<version>
mise lock
mise install
```

## Workflow

Development is spec-driven, on [OpenSpec](https://github.com/Fission-AI/OpenSpec),
through Claude Code's `/opsx:*` commands.
Each change is one OpenSpec change on its own `feature/<id>` branch,
and `/opsx:sync` runs before `/opsx:archive`.
A Claude Code session either plans or implements, never both,
and commits everything it wrote before `/clear`.
A plan is a commit of its own, and so is a replan:
on `develop` until the change has its branch, on `feature/<id>` from then on.
A change is planned one of two ways.

**Ahead,**
when it is planned now and implemented later,
and only if it depends on nothing still unlanded.
Each plan is proposed and committed on `develop` alone,
and may wait there, unimplemented, until its turn:

```shell
git flow feature start <id>    # right before /opsx:apply
git flow feature finish <id>   # after /opsx:sync, /opsx:archive and the commits
```

**Just in time,**
when it is implemented right after it is planned,
and always for a change that builds on one still in flight,
once that one has landed on `develop`.
The branch comes first,
and the plan is proposed and committed on it:

```shell
git flow feature start <id>    # then /opsx:propose "<id>"; /opsx:apply in the next session
git flow feature finish <id>   # after /opsx:sync, /opsx:archive and the commits
```

The implementation lands as one commit or several split by meaning.
A decision the plan did not make stops it,
and so does a change wider than its specs:
the code is committed as `wip(<id>): …`,
a fresh session replans it —
the decision made, or the change narrowed and the rest made a change of its own —
and before the finish the `wip` commits are split again by meaning,
without the code the narrowed change no longer covers.

Browse specs and changes with `mise exec -- openspec view`.

To install the agent skills into `.claude/skills/` in a fresh clone,
or to update them:

```shell
mise exec -- npx skills add petr-korobeinikov/skills --skill '*' --copy --agent claude-code -y
```

## Release

One command on a clean `develop` cuts a release and pushes it:

```shell
mise run release 0.2
```

It starts `release/0.2`,
sets `fap_version` in `application.fam`,
regenerates `CHANGELOG.md` and `catalog/changelog.md` with git-cliff,
builds the app, and commits the three files as `chore: release 0.2`.
Then it finishes the branch, which tags `v0.2`,
returns to `develop`,
and pushes `main`, `develop` and `v0.2` to `origin` in one atomic push.

It refuses a name other than the latest tag's `MINOR` plus one,
a branch other than `develop`, and a worktree that is not clean.
A failure before the release commit undoes the run:
the files restored, `develop` checked out, `release/0.2` deleted,
so the fixed run is the same command.
After the commit nothing is undone.
A finish stopped on a conflict resumes once it is resolved,
and a failed push is rerun as is:

```shell
mise exec -- git flow finish --continue
git checkout develop
git push --atomic origin main develop v0.2
```

The tag runs `.github/workflows/release.yml`:
`ci.yml`'s jobs first,
then the tag checked against `fap_version`,
the app built, and the Release published with its notes and `f0_claude_remote.fap`.

A published Release is immutable, and its tag name is spent.
If a run fails before publishing,
delete the tag, fix the cause, and cut the same name again.

A GitHub Release's notes are its own section alone:

```shell
git checkout v0.2
mise exec -- git-cliff --current --strip all
```

## Hooks

[prek](https://prek.j178.dev/) holds the gate before a commit is allowed.
It is pinned in `mise.toml` and configured in `prek.toml`,
which gives each rule's reason.
Every hook is one of prek's own Rust implementations
or a binary pinned in `mise.toml`,
so this repository clones no hook repositories
and pins no third-party revs.

File-hygiene hooks run at `pre-commit`,
and so does [shellcheck](https://www.shellcheck.net) over every shell script,
the tasks under `.mise/tasks/` included.
actionlint hands it the workflows' `run` steps.
Three more read the message at `commit-msg`:
a subject of at most 72 characters,
in Conventional Commits,
and no line that credits an AI agent or a model.

```shell
mise exec -- prek install           # wire both git hooks
mise exec -- prek run --all-files   # every pre-commit hook over the whole tree
```

Run `prek install` in a fresh clone and after a prek upgrade:
the generated hooks pin the path of the prek binary they were installed from,
and that path carries the version.

## CI

`.github/workflows/ci.yml` runs on every push to `develop` and `main`
and on every pull request,
and `release.yml` calls it before it publishes,
in three parallel jobs on `ubuntu-24.04`.
Each step is a command to run before pushing:

| Job | Steps |
|---|---|
| `test` | `mise run test` |
| `build` | `mise exec -- sh -c 'ufbt update --branch "$FLIPPER_SDK_VERSION"'`, `mise run build`, `mise exec -- ufbt lint` |
| `hygiene` | `mise exec -- prek run --all-files`, `mise exec -- actionlint -pyflakes=` |

On the Mac, `mise run setup` stands in for `build`'s first step.
Actions are pinned by commit, mise by version and the tools by `mise.lock`,
and no cache is restored;
the runner image, with its `cc` and `gh`, stays unpinned.
