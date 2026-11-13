# Design

## Context

See `proposal.md` - Why. The two Purposes sit on line 5 of their specs, each a paragraph of one line,
as the other 25 do.

## Goals / Non-Goals

**Goals:** both specs pass `openspec validate --specs --strict`.

**Non-Goals:** a requirement, a scenario, an archived change, or a check that runs `openspec validate` in CI or the hooks.

## Decisions

None beyond the proposal's: no layer, file or test is in play,
and the edit has one way to make, by hand in the main spec, as the archive leaves an existing Purpose alone.

## Risks / Trade-offs

- A Purpose drifts from its requirements as they change → it names what the requirements cover, not their numbers,
  and a plan that adds a board or a step reads the Purpose with the spec.
