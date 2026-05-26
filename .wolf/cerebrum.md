# Cerebrum

> OpenWolf's learning memory. Updated automatically as the AI learns from interactions.
> Do not edit manually unless correcting an error.
> Last updated: 2026-05-26

## User Preferences

<!-- How the user likes things done. Code style, tools, patterns, communication. -->

## Key Learnings

- **Project:** quassimodo — Quoridor game (C++ / Irrlicht / Boost::Python) where two Python AI agents compete. Dormant since 2010, being resurrected.
- **VCS history:** Two Mercurial clones, NOT named branches. `quassimodo/` (main, hg tip `ee96566b`, rev 232) and `quassimodo.concurso/` (concurso, hg tip `920aa5f`, rev 332). Both on hg branch `default`. Shared root `08318f39` (2010-03-08), 232 shared changesets. They fork at last-common changeset `110401bfd` (main rev 231 = concurso rev 250). Topology is INTERLEAVED — concurso pulled from main repeatedly, so main's commits are woven through concurso's DAG.
- **Build artifacts in history:** `.o`, `.so`, `.pyc` were committed early (first commit msg: "incluidos los binarios :S") then removed; they persist in history. Game assets (textures .jpg/.png/.bmp, models .3ds, fonts, doc .odt) are legitimate project files.

## Do-Not-Repeat

<!-- Mistakes made and corrected. Each entry prevents the same mistake recurring. -->
<!-- Format: [YYYY-MM-DD] Description of what went wrong and what to do instead. -->

- [2026-05-26] hg→git: do NOT use hg-fast-export.sh on this machine — the active python3 (pyenv 3.13) cannot `import mercurial`. Use git-cinnabar instead (self-contained, no mercurial python module needed).
- [2026-05-26] Don't convert the two clones to git separately and graft — topology is interleaved. Unify into one hg repo first (`hg pull`), then convert once so the fork point is reconstructed correctly from the DAG.

## Decision Log

<!-- Significant technical decisions with rationale. Why X was chosen over Y. -->

- [2026-05-26] hg→git migration strategy (user-approved decisions):
  - **Binaries:** Strip regeneratable build artifacts (.o/.so/.pyc, NetBeans private/) from history via `git filter-repo`; KEEP game assets (textures/models/fonts/.odt). Add .gitignore going forward.
  - **Layout:** Flatten project to git repo root (hg paths are already root-relative, so natural).
  - **Location:** Convert in place at `/Users/fabzter/src/quassimodo`; keep `.hg` clones as a safety net until conversion is verified, then remove.
  - **Branches:** main clone tip → `main`; concurso clone tip → `concurso`; they diverge at `110401bfd`.
  - **Tooling:** git-cinnabar (hg→git) + git-filter-repo (artifact strip). Unify clones with `hg pull` first.
