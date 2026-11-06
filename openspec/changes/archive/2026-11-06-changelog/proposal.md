# Proposal

## Why

The repository keeps no record of what a release changes:
42 Conventional Commits sit on `develop` under no tag,
and the release rule in `openspec/config.yaml` names a release and sets its `fap_version`
but says nothing of what it lists.
`claude-shim`, the model for this repository's dressing, commits no changelog.
git-cliff is the first of four items the owner left before `v0.1`,
ahead of `ci.yml`, `release.yml` and `dependabot.yml`.

Decisions, settled by the owner:

- **git-cliff writes a changelog file, committed with the version bump on the `release/` branch** (2026-11-06),
  not a hand-kept file nor notes alone.
- **A GitHub Release shows only its own changes** (2026-11-06),
  not the whole file.
- **`release.yml` is a change of its own, after `ci.yml`** (2026-11-05):
  this change gives it the notes command.

Decisions proposed here, for the owner to strike:

- **`CHANGELOG.md`,**
  not `changelog.md`:
  like `README.md`, `CONTRIBUTING.md` and `SECURITY.md`.
- **The release commit regenerates the whole file,**
  not `--prepend`:
  the file stays a function of the history and `cliff.toml`, so a rerun proves it current and no hand edit survives.
- **Notes from `git-cliff --current --strip all` on the tag,**
  not cut from `CHANGELOG.md`:
  one template renders both from the same commits with no parser of ours,
  and past the first tag an untagged commit errors out rather than show an older section.
- **The release commit and `chore(openspec)` commits stay out of every section,**
  not listed:
  the file predates its release commit, so listing that commit would part the notes from the file,
  and a plan and its archive restate their change, 11 of the 42 commits.
- **`claude-shim`'s `cliff.toml`, adapted,**
  not a git-cliff built-in template:
  the two repositories read alike, and the design lists what changes.
- **`GIT_CLIFF_OFFLINE` in `mise.toml`,**
  not `[remote] offline` in `cliff.toml`, which leaves the update check on:
  every run through mise stays off the network, `release.yml`'s included.
- **No mise task,**
  not `.mise/tasks/changelog`:
  the version is typed by hand three times anyway,
  and the release commit's diff shows the section's name.
- **The config's release rule names `CHANGELOG.md`,**
  beside `CONTRIBUTING.md`'s commands:
  the config reaches every agent, so none edits the file in a feature commit.

## What Changes

- `mise.toml` pins `git-cliff` 2.14.2 and sets `GIT_CLIFF_OFFLINE`; `mise.lock` gains it.
- New `cliff.toml`.
- `CONTRIBUTING.md` gains a Release section; the config's release rule names the file.
- No `CHANGELOG.md` yet: release `0.1`'s commit writes the first.

## Capabilities

### New Capabilities

- `changelog`: the release history git-cliff writes into `CHANGELOG.md`, and the notes it prints for one release.

### Modified Capabilities

None.

## Impact

Audit, 2026-11-06, against OSV, GHSA and deps.dev.
Snyk covers no Rust, and the owner dropped Socket.
Stage 4, no host mount: `cargo tree --locked` over the release's lock,
then the Linux binary run with the network cut.

| Dependency | Version | License | Source | Known vulnerabilities | Transitive artifacts |
|---|---|---|---|---|---|
| git-cliff | 2.14.2, 2026-09-18 | MIT OR Apache-2.0 | aqua, GitHub release; sha512, GPG `4A92FA17B6619297` | 0 in git-cliff; 32 in its dependencies, below | 321 crates in the binary; no install-time code |

Findings:

- **Every run asks crates.io for a newer version**
  (`update-informer`, a default feature)
  and writes `~/.cache/update-informer-rs/crates-git-cliff`, outside the project.
  `GIT_CLIFF_OFFLINE=true` skips the check and the remote integrations:
  so set, sandboxed runs wrote nothing outside the repository.
  clap refuses `1`; it takes `true` or `false`.
- **19 of the 321 crates carry 32 advisories,**
  3 of them unmaintained notices:
  12 sit in the TLS, HTTP and cache code an offline run at the root never reaches,
  `zstd`'s three also unpack the compiled-in example configs,
  and `crossbeam-epoch`, `libz-sys`, `pulldown-cmark` and `rand` take only our history and `cliff.toml`.
  Accepted on the toolchain's ground: the project exposes nothing outward.
- **`mise.lock` will record no provenance:**
  aqua's registry declares only the release's own `.sha512`, unlike actionlint's attestations;
  the GPG check here covers the macOS and Linux assets the lock pins, gnu and musl.
