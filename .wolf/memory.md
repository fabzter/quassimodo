# Memory

> Chronological action log. Hooks and AI append to this file automatically.
> Old sessions are consolidated by the daemon weekly.

## Session: 2026-05-26 15:26

| Time | Action | File(s) | Outcome | ~Tokens |
|------|--------|---------|---------|--------|

## Session: 2026-05-26 15:32

| Time | Action | File(s) | Outcome | ~Tokens |
|------|--------|---------|---------|--------|
| 16:11 | Created .gitignore (build artifacts + scratch) | .gitignore | created | ~75 |
| 16:30 | hg->git migration: unified 2 hg clones, cinnabar convert, strip artifacts (filter-repo --prune-empty=never), LFS assets, landed in place | .git, .gitattributes | main=233/concurso=333 commits, fork preserved, lfs fsck OK, code pack 744K + 75M LFS | ~migration |
| 16:30 | PENDING user confirm before deleting originals | quassimodo/, quassimodo.concurso/, .migration/ | awaiting go-ahead | — |
| 16:16 | Session end: 1 writes across 1 files (.gitignore) | 1 reads | ~1243 tok |
| 16:20 | Edited .gitignore | scratch() → clones() | ~34 |
