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
- `check-workflow` runs `.github/workflows/build.yml` through `act`,
  contained, against colima's Docker socket unless `DOCKER_HOST` says otherwise.

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
Each change is one OpenSpec change,
and every plan is one commit.
It is planned one of two ways,
and `/opsx:sync` runs before `/opsx:archive` either way.

**Ahead.**
Each plan is proposed on `develop` and committed alone,
and may wait there, unimplemented, until its turn.
Its implementation is one commit on its own branch:

```shell
git flow feature start <id>    # right before /opsx:apply
git flow feature finish <id>   # after /opsx:sync, /opsx:archive and the commit
```

**Just in time.**
The branch comes first,
and the plan, the code and the archive go in as one commit:

```shell
git flow feature start <id>    # then /opsx:propose "<id>" and /opsx:apply on it
git flow feature finish <id>   # after /opsx:sync, /opsx:archive and the commit
```

Every implementation runs in a clean Claude Code session,
which is cleared with `/clear` once its branch is finished.

Browse specs and changes with `mise exec -- openspec view`.

To install the agent skills into `.claude/skills/` in a fresh clone,
or to update them:

```shell
mise exec -- npx skills add petr-korobeinikov/skills --skill '*' --copy --agent claude-code -y
```

## Release

A release's own commit, the last on its `release/` branch,
sets `fap_version` in `application.fam` and regenerates `CHANGELOG.md`.
git-cliff runs from the repository root: below it, it keeps only that folder's commits.

```shell
git flow release start 0.2
# fap_version="0.2" in application.fam
mise exec -- git-cliff --tag v0.2 --output CHANGELOG.md
git add application.fam CHANGELOG.md
git commit -m "chore: release 0.2"
git flow release finish 0.2
```

A GitHub Release's notes are its own section alone:

```shell
git checkout v0.2
mise exec -- git-cliff --current --strip all
```

## Hooks

[prek](https://prek.j178.dev/) holds the gate before a commit is allowed.
It is pinned in `mise.toml` and configured in `prek.toml`,
which gives each rule's reason.
Every hook is one of prek's own Rust implementations,
so this repository clones no hook repositories
and pins no third-party revs.

File-hygiene hooks run at `pre-commit`.
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
