# ZNC ROM (Read Only Member)

ZNC ROM is ZNC module that can be set to read-only to the specified channel.

## Requires

- znc 1.0 or later with development packages.
    - from [Source Code](http://wiki.znc.in/ZNC).
    - from package manager
        - eg. Ubuntu, Debian, etc..

          `$ apt-get install znc-dev`

## Installation

Compile and copy the compiled module (rom.so) into your ZNC directory.

    $ znc-buildmod rom.cpp
    $ cp rom.so ~/.znc/modules/

## Configuration

In the *status query window, send

    /msg *status loadmod rom #ruby-ja #emacs-ja

or write your config file.

    <User hoge>
        ...
        LoadModule = rom #ruby-ja #emacs-ja
        ...
    </User>

## Command

    gongo: help
    *rom: +---------+-----------+----------------------+
    *rom: | Command | Arguments | Description          |
    *rom: +---------+-----------+----------------------+
    *rom: | Add     | <channel> |                      |
    *rom: | Del     | <channel> |                      |
    *rom: | Help    | search    | Generate this output |
    *rom: | List    |           |                      |
    *rom: +---------+-----------+----------------------+

### List channel

    /msg *rom list
    current read-only channels: #ruby-ja #emacs-ja

### Add channel

    /msg *rom add #python
    current read-only channels: #python
    /msg *rom add #perl
    current read-only channels: #python #perl

If you try to add an existing channel

    /msg *rom list
    current read-only channels: #hoge
    /msg *rom add #hoge
    #hoge is already been added.

### Delete channel

    /msg *rom list
    current read-only channels: #hoge #fuga
    /msg *rom del #fuga
    current read-only channels: #hoge

## License

MIT LICENSE. see LICENSE
