# Design

## Context

See `proposal.md` for why.
No app layer is touched: the change lands in tooling and docs, and nothing under `src/` moves.

`git flow release finish` merges the release into `main`, tags that merge and merges `main` back into `develop`,
as the git-flow-next skill states; the aborted release of 2026-11-05 made both merges.
Every tag so stays reachable from `develop`.

What the plan rests on in git-cliff 2.14.2, and where it is stated:

| Fact | Documented | Source |
|---|---|---|
| `--tag` names the untagged commits' section, `--current` takes the tag on `HEAD`, `--strip all` drops header and footer | `usage/args.md`, `usage/examples.md` | |
| A release holds the commits its tag reaches and the previous tag does not | no | `lib.rs:388` |
| The `--tag` section is dated by the clock, a tagged one by its commit's committer time, in UTC | no | `lib.rs:457`, `:468` |
| Past one tag, `--current` on an untagged commit exits with `No tag exists for the current commit` | no | `lib.rs:99` |
| Past one tag, `--current` on the first tag exits with `No suitable tags found` | no | `lib.rs:93` |
| Run below the root, only commits touching that folder are kept | no | `lib.rs:334` |
| `trim` trims every template line; header and footer each get a newline appended | `trim` only | `template.rs:29`; `changelog.rs:625`, `:649` |
| `GIT_CLIFF_OFFLINE` also skips the update check, and `[remote] offline` does not | remote access only | `main.rs:41` |

A bump can break the undocumented rows: it reruns the dry run in `tasks.md`.

## Goals / Non-Goals

**Goals:** one command regenerates the file in the release commit, one prints a release's notes, neither touches the network.

**Non-Goals:** `release.yml`, with its build, its tag check against `fap_version` and its upload; a `hotfix/` release, which the config's rule does not name; the first release itself.

## Decisions

| File | Its one task |
|---|---|
| `cliff.toml`, new | Renders the history as changelog sections |
| `mise.toml` | `git-cliff = "2.14.2"` between `actionlint` and `node`; `GIT_CLIFF_OFFLINE = "true"` after `OPENSPEC_TELEMETRY` |
| `CONTRIBUTING.md` | A `## Release` section before `## Hooks`, below |
| `openspec/config.yaml` | The release paragraph's last sentence, below |

`cliff.toml` is `claude-shim`'s, with:

```text
header          "# Changelog\n"                    was ""
body            opens with a blank line;           was ending in `{% endfor %}\n`
                ends in `{% endfor %}\`
footer          removed                            was ""
tag_pattern     '^v[0-9]+\.[0-9]+$'                was '^v[0-9]+\.[0-9]+\.[0-9]+'
commit_parsers  opens with
                { message = '^chore: release [0-9]+\.[0-9]+', skip = true },
                { message = '^chore\(openspec\)', skip = true },
```

Its comments are rewritten to the config's rule: the separator's place and the two skips say why; the rest says nothing.

- **The blank line opens each section and no footer is set,**
  not `claude-shim`'s body as is:
  its trailing blank line and the newline after `footer = ""` would end the file in two blank lines,
  which `end-of-file-fixer` rewrites.
- **A bare `git-cliff` pin,**
  not `"aqua:orhun/git-cliff"` as in `claude-shim`:
  the mise skill puts a single-binary CLI under its bare name, as `act` and `actionlint` here,
  and `mise registry git-cliff` resolves to that backend alone.
- **`--current` for the notes,**
  not `claude-shim`'s `--latest`:
  on the tag both print the same, while `--latest` takes the newest tag wherever it runs,
  so a rerun for an older tag would print another release.
- **A comment on `GIT_CLIFF_OFFLINE`,**
  where `mise.toml` has none:
  git-cliff documents it for remote repositories alone, and dropping it brings the update check back.

`CONTRIBUTING.md`, the new section:

````text
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
````

`openspec/config.yaml`, in place of ``and is set on the `release/` branch, before its finish.``:

```text
and is set on the `release/` branch, before its finish, by the commit
`chore: release 0.1`, the branch's last. That commit also regenerates
`CHANGELOG.md` with git-cliff, and nothing else writes that file.
```

**Test seam.**
A scratch clone outside the repository, its remote removed, cuts two releases with git-flow-next:
`0.1`, then `0.2` after one empty `feat` commit on `develop`.
git-cliff runs there through this project's mise, with this tree's `cliff.toml` passed by `-c`;
each scenario in the spec is read off that clone.
The network scenarios run in the project itself.

## Risks / Trade-offs

- A commit after the release commit escapes `CHANGELOG.md` → the release commit goes last, as the config and `CONTRIBUTING.md` say; one that slips through still reaches the notes.
- A finish on a later UTC day than the release commit dates the section by the merge → finish the same day; the next regeneration shows any shift in its diff.
- `mise.lock` pins checksums with no provenance → the audit's GPG check of the same assets, and task 1.1's comparison.
