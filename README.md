# Database project for the mentorship program

Aim is to make progress on building a fast (ish) time-series database from scratch, in C++.

# Participants

- Joe Murdoch
- Perry Hui
- Kamai Jackson-Wade
- Alex Do (mentor)

# Development

You need to have [bazel](https://bazel.build/install) installed to build the project.

Then, to check everything is in order:

```sh
bazel build //src/...
```

When developing in general, you should endeavour to minimise the amount of code you write, and write tests to demonstrate that what you have written works.

See in `storage/structures/btree` for an example test built with GTEST, the google test suite.

# compile_commands.json

Regenerate with:

```sh
bazel run @hedron_compile_commands//:refresh_all
```
