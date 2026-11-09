## MODIFIED Requirements

### Requirement: Architecture shows the layers and a press's path
`docs/internals/architecture.md` SHALL give the three layers and the rule that includes point down,
each source file's one task,
and the files a press passes through, with the queues between threads.

#### Scenario: Files
- **WHEN** the page is read beside `git ls-files src`
- **THEN** it names every file there once, with its task in one sentence

#### Scenario: Path
- **WHEN** the press's path is read
- **THEN** it runs `screen.c`, `app.c`, `keymap_input.c`, `keymap.c`, `actions.c`, `transport_hid.c`,
  then `transport_bluetooth.c` or `transport_usb.c`,
  and names the input and link queues

## ADDED Requirements

### Requirement: USB explains the switch
`docs/internals/usb.md` SHALL explain the switch:
why it is a hold on `No link` and not a second application or the cable,
why the console goes, the keyboard's name, the order of the switch, of the return and of leaving,
why the link queue is emptied, the status callback's interrupt, and the 60 s counted afresh.

#### Scenario: Marks
- **WHEN** each fact is read
- **THEN** it names its kind, documented, firmware 1.5.1's sources or seen,
  and links its source: the documentation page, the file and line at tag `1.5.1`, or the archived task
