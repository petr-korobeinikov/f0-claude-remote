# Design

## Context

See `proposal.md`, Why.
No app layer is touched:
the change lands in the docs workflow, two contributor pages, `docs-deploy`'s Purpose and one GitHub setting.

| Fact | Where |
|---|---|
| `main` is `v0.1`, `558cb3e`, and its `docs/guide/` holds no `usb.md` | `git rev-parse main 'v0.1^{commit}'`, `git ls-tree main docs/guide/`, 2026-11-11 |
| Only `mise run release` moves `main`, in the push it ends on | `.mise/tasks/release:78`; `release` spec |
| `v0.1`'s site builds with `develop`'s tools; only `zensical.toml`'s two USB pages differ | `git diff --stat v0.1 develop -- mise.toml mise.lock .mise zensical.toml docs/templates` |
| A `workflow_dispatch` runs only if its workflow is on the default branch; `--ref` picks the branch it runs on, with that branch's workflow | GitHub Docs, Manually running a workflow |
| An environment a workflow creates has no protection rule; GitHub advises limiting `github-pages` to the default branch | GitHub Docs, Managing environments for deployment; Configuring a publishing source |
| An event `GITHUB_TOKEN` causes starts no workflow, `workflow_dispatch` and `repository_dispatch` aside | GitHub Docs, Triggering a workflow |

## Goals / Non-Goals

**Goals:** the specs'.

**Non-Goals:** the version selector, a change of its own;
`README.md`, `SECURITY.md` and the guide;
`ci.yml`, whose `docs` job keeps building `develop`'s pages on each push.

## Decisions

Each choice with an alternative is in the proposal's Why.

| File | Its one task | What changes |
|---|---|---|
| `.github/workflows/docs.yml` | Publishes the site on GitHub Pages | Line 5, under a comment |
| `docs/contributing/hooks-and-ci.md` | Tells a contributor what gates a commit and what checks a push | Lines 58-59 |
| `docs/contributing/release.md` | Tells a contributor how a release is cut | A line after `in one atomic push.`, line 19 |
| `openspec/specs/docs-deploy/spec.md` | States what `docs-deploy` requires | Its Purpose, which sync leaves alone |

On GitHub, `github-pages` gains a branch rule for `main` only if task 1.2 finds it refusing `main`.

`docs.yml`'s trigger:

```yaml
on:
  push:
    # main moves only with a release, so the site shows the latest Release.
    branches: [main]
  workflow_dispatch:
```

`hooks-and-ci.md:58-59` become:

```markdown
`.github/workflows/docs.yml` builds the site on every push to `main`,
which only a release makes,
and publishes it to GitHub Pages,
so the site shows the latest Release.
A manual run publishes the branch it runs on, so it picks `main`.
```

`release.md` gains, after line 19:

```markdown
The push of `main` publishes the site, through `.github/workflows/docs.yml`.
```

`docs-deploy`'s Purpose becomes:

```markdown
Publishes the documentation site on GitHub Pages,
with Pages' permissions held by the publishing job alone.
```

Test seam: the published state is the only place the change shows.
`gh api` reads the environment, `curl` reads the site,
and `v0.1`'s pages differ from `develop`'s where USB came in: `guide/usb/` answers 404 on them.
`mise run docs-build` builds the edited pages.

## Risks / Trade-offs

- `v0.1`'s `docs.yml` fails on today's runners → it takes `develop`'s tools, which build on every push (the facts).
- The site shows a release's pages before its Release is published → `publish` ends that run, and a failed run is cut again under the same name (`docs/contributing/release.md`), which moves `main` again.
