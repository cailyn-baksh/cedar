# Contributing

## Improving Documentation

## Bugfixes

If you discover a bug with this library and wish to fix it, first check Issues
to see if this bug has been reported. If it has been reported and no one else
has said they are working on fixing it, leave a comment saying that you're 
working on it. If it hasn't been reported, open a new issue describing the bug,
and say in your issue that you are going to fix it yourself.

Once you fix the bug, make sure you update the version numbers appropriately,
then submit a pull request and add a comment to the issue mentioning your pull
request.

If you abandon a bug fix, add another comment on the issue saying so.

## New Features

When you finish implementing a new feature, submit a pull request. Pull
requests for new features have some additional requirements which must be met
for the request to be merged.

- Your feature should be thoroughly documented
  - Each function, structure, constant, enum, global variable, etc. should
    have a comment documenting its functionality.
  - Your code should have additional comments explaining how it works.
  - You should add a well-documented example to the `examples/` folder.
  - You should document the outward-facing components of your feature
    thoroughly in a new file or existing file in the `docs/` folder.
- Version numbers should be updated

## Versioning

This project uses a simple `MAJOR.MINOR` versioning scheme. The constants
`LIBCEDAR_MAJOR_VERSION` and `LIBCEDAR_MINOR_VERSION` provide the version
numbers.

The major version number is incremented after breaking API changes or after
major functionality changes (i.e. changes that could cause programs to behave
differently). User code written for a specific major version number should be
compatible with any version of the library with the same major version.

The minor version number is incremented when new features are added, after
minor functionality changes (i.e. changes that would cause existing programs
to behave differently but not unexpectedly), and after bugfixes that affect
how the program runs (very minor fixes like graphical bugs do not change the
version number).

## Pull Requests

When you're done making a change, submit a pull request. Your pull request
should broadly describe what it does in its title, and provide a summary of
the changes made in the description. Any relevant issues should be mentioned
in the title.
