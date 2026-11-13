## ADDED Requirements

### Requirement: The core decides which link is in use
Which of the links of `bluetooth-link` and `usb-link` the remote is on SHALL be decided in `src/core/` by the keymap,
which answers it after every event, and SHALL be named there by no action and by no call on the transport.
The keymap's suite SHALL pin when the link moves and when it stays,
and the platform SHALL put the transport on that link, as it shows it on the screen,
at the opening as after an event.

#### Scenario: Moved
- **WHEN** Right's hold fires on the no-link screen, from a press made there
- **THEN** the core answers the other link, and no action

#### Scenario: Stayed
- **WHEN** any other event comes:
  another button's event on the no-link screen, Right tapped there,
  its hold fired there from no press or from one made on another screen,
  Right held on another screen, or a change of mode
- **THEN** the core answers the link it held

#### Scenario: Named by no action and no call
- **WHEN** `git grep -l -E 'Usb|Bluetooth' -- src/core` runs
- **THEN** it prints `src/core/keymap.c` and `src/core/link.h` alone

#### Scenario: The platform follows
- **WHEN** `git grep -n 'keymap_link' -- src/platform` runs
- **THEN** it prints four lines of `app.c`:
  the two that open the screen and the transport, and the two that set them after an event

## MODIFIED Requirements

### Requirement: The core decides when the wait is counted
What the 60 s of `bluetooth-link` and `usb-link` are counted from, and what ends the count,
SHALL be decided in `src/core/`, which answers every event with what the count does:
it starts, it stops, or it stays as it is.
The keymap's suite SHALL pin that answer,
and the platform SHALL start and stop its timer by it alone, keeping no count of its own.

#### Scenario: Started
- **WHEN** the application opens, the link drops, a press starts over on the stopped screen, or Right's hold switches the link
- **THEN** the core answers that the count starts

#### Scenario: Stopped
- **WHEN** the link comes up, or the 60 s end
- **THEN** the core answers that the count stops

#### Scenario: Left as it is
- **WHEN** any other event comes:
  a button event with no link, a drop reported again, OK released after a cut,
  or an event with the link up or on the stopped screen that starts nothing over
- **THEN** the core answers that the count stays as it is

#### Scenario: Nothing counted in the platform
- **WHEN** `git grep -n 'wait_ms[^(]' -- src/platform` runs
- **THEN** it prints nothing
