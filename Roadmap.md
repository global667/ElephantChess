## Roadmap, Mittwoch der 16.08.2023
Every version should release in a weekly to two weekly cyclus.

### Known Bugs
* dont respect move right after take back 
* dont compliane the game rules if engine is stopped
* take-back doen't works
* load pgn doesn't fill moves right need correct init
* Elephant moves wrong [ok]
### Version 0.3
- show available moves [ok]
- add Toolbar [ok]
- change red/black on the upper side [ok]
- delete pgnlibrary deps [ok]
- selectable engines [ok]
### Version 0.4
- smooth piece animation [ok]
- save settings
- add euro symbols [ok]
- add markers [ok]
- Make board authentic (doubled outter lines, 
doubled cutting points for cannons and soldiers)
- different notations (classic, euro, chinese)
### Version 0.5
- improve move list
    - add treeview
    - make moves on list selectable
- add engines fully support (read the options from uci...)
### Version 0.xx    
- start writing own engine
- svg pieces
- icons [ok]
- add languages: englisch, spanisch, chinese
- Elo curves and calculator
- Beginner mode
- delete single marker or figure
- care about more than one game in a file on pgns
- add engines 
- set board pieces
- add system tray (QSystemTrayIcon)
----------------------------------------------------
How to Markdown
---------------
# First Level Heading

Paragraph.

## Second Level Heading

Paragraph.

- bullet
+ other bullet
* another bullet
    * child bullet

1. ordered
2. next ordered

### Third Level Heading

Some *italic* and **bold** text and `inline code`.

An empty line starts a new paragraph.

Use two spaces at the end  
to force a line break.

A horizontal ruler follows:

---

Add links inline like [this link to the Qt homepage](https://www.qt.io),
or with a reference like [this other link to the Qt homepage][1].

    Add code blocks with
    four spaces at the front.

> A blockquote
> starts with >
>
> and has the same paragraph rules as normal text.

First Level Heading in Alternate Style
======================================

Paragraph.

Second Level Heading in Alternate Style
---------------------------------------

Paragraph.

[1]: https://www.qt.io 