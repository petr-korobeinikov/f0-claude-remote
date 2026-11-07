---
description: How a change is planned, implemented and landed with OpenSpec, git-flow and Claude Code.
---

# Workflow

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

## Commits

A commit message is a Conventional Commit, in English,
its subject at most 72 characters, and no line of it credits an AI agent or a model:
[Hooks](hooks-and-ci.md#hooks) check all three at `commit-msg`.
The full rules, which agents read, are the Workflow section of
[`openspec/config.yaml`](https://github.com/petr-korobeinikov/f0-claude-remote/blob/develop/openspec/config.yaml).
