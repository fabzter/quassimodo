# Kickoff prompt for the next session (D2.2a · M1)

> Paste the block below as the first message of the forked/continued session. It is a
> one-shot kickoff for **executing the D2.2a Milestone-1 plan**. The durable guide is
> `ONBOARDING.md` (Operating character + §3 Anti-drift discipline + §7); this prompt just
> points there and states the immediate task. Once M1 ships, this file is stale — the next
> session's task is M2, derivable from `ROADMAP.md`.

---

Continue the Quassimodo revival — you are a continuation of the prior sessions, not a fresh
contractor. **The repo on disk is the source of truth and may be AHEAD of anything you think
you remember in-context — trust `git log` and the docs, not memory.** Start with
`git log --oneline -10` and `git status` to see the real state.

**Orient by reading, in this order:**
1. `ONBOARDING.md` — read the **"Operating character"** section, **§3 (incl. "Anti-drift
   discipline")**, and **§7 (Immediate next action)** carefully; skim §4 (build playbook) + §5 (conventions).
2. `docs/superpowers/ROADMAP.md` — the live phase table (D1…D2.1 done; D2.2a·M1 is next).
3. `docs/superpowers/specs/2026-06-02-d2.2a-animation-camera-atmosphere-design.md` — the agreed D2.2a design.
4. `docs/superpowers/plans/2026-06-02-phase-d2.2a-m1-movement-framing.md` — the **7-task M1 plan you will execute**.
5. `.wolf/cerebrum.md` (learnings/decisions/do-not-repeat) and `KNOWN-BUGS.md` (KB-001 Minimax SIGSEGV; KB-D2-001..006).

Hold the WHOLE arc: a dormant 2010 Quoridor game being revived to a reproducible, *playable*
3D game on modern macOS arm64. D2.1 shipped a graphical match with **instant** moves; D2.2a·M1
makes moves **animate**.

**Adopt the operating character (this is how the prior sessions worked, and why it worked):**
truth over agreement (push back with evidence, even on me); **verify before claiming — rebuild,
run, and read the actual output/screenshot with your own eyes, never trust a subagent's report**;
confirm only genuine forks (one question at a time, with a recommendation); care with
irreversible/host actions; decompose ruthlessly and ship value early; **curate subagents and
prevent drift** (ONBOARDING §3); keep cerebrum/ROADMAP/ONBOARDING/KNOWN-BUGS alive.

**Methodology — invoke superpowers skills via the `Skill` tool.** D2.2a·M1 is **already
brainstormed, spec'd, and planned — do NOT re-brainstorm it.** Go straight to
`superpowers:subagent-driven-development` and execute the committed 7-task M1 plan task-by-task.
Apply the **anti-drift discipline** from ONBOARDING §3 on every dispatch:
- Inline the full corrected code + exact commands + expected output into each subagent prompt
  (never "go read the file"); bake in the env gotchas, conventions, and "this is a port —
  BLOCK, don't mock or change game logic."
- Verify every IrrlichtMt API claim against the fork headers
  (`/nix/store/…-irrlichtmt-1.9.0mt15/include/irrlichtmt/*.h`) **before** baking it into a
  prompt; when a task surfaces a new API reality, fold it into the next task's prompt.
- Adaptively re-plan between tasks (patch + commit the downstream plan as findings land).
- **Verify every gate yourself**: rebuild, run (capture `$?`, never `| tail`), `otool -L`, and
  for any visual gate **Read the screenshot PNG with your own vision**. The M1 vision gate is a
  pawn caught **mid-jump between cells** — that is the proof moves animate, not teleport.
- Front-load latent risk (e.g. `-Wreturn-type` fall-off-the-end `bool`s = D1-SIGTRAP class).
- Model selection: **Sonnet** for the mechanical/new-class tasks (1, 2, 4, 7), **Opus** for the
  `PartidaGrafica` refactor + driver + vision gate (3, 5, 6). Two-stage review (spec then
  quality) on substantial tasks; controller-review trivial ones — but never skip verification.

**Conventions (hard constraints — override defaults):**
- **No AI/LLM attribution anywhere** (commits, PRs, code, comments).
- Git identity is repo-local `fabzter` — plain `git commit`. **Explicit `git add <files>`**,
  never `git add -A` (`.wolf/buglog.json` auto-churns — commit or `checkout --` it, don't sweep it).
- To push: `gh auth switch --user fabzter`, push/PR, then `gh auth switch --user fabriziohernandez`;
  verify with `gh api user --jq .login` before pushing. **PR per milestone; you do NOT merge — I do.**
- `nix`/tools are NOT on PATH non-interactively: prefix
  `. /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh && …`; build via
  `nix develop -c bash -c '…'`. Never pipe a running binary through `| tail`/`| head`.
- Run `aplicacion` from the **repo root** with `lib/Reglas.so` staged
  (`mkdir -p lib && ln -sf "$PWD/build/src/AgenteWrapper/Reglas.so" lib/Reglas.so`); gate agents
  = the proven clean pair `bin/agenteCamina.py bin/agenteCamina2.py` (Minimax SIGSEGVs — KB-001).

**Current task:** execute the **D2.2a·M1 plan** (movement & framing) subagent-driven. Task 1 cuts
`feat/d2.2a-m1-animation` from `main`. Gate = a graphical match that plays to "Hay un ganador!"
with **visibly animated pawn jumps** (verified by reading a mid-jump screenshot), a clean static
3/4 camera, `consola` regression intact, and no `boost_python` in `otool -L`. Open the PR when
green. After M1 merges: plan + execute **D2.2a·M2** (atmosphere & polish — billboard flames, blob
shadows, ground+sky, cosmetic), then **D2.2b** (menu/GUI + input + interactive camera).

Confirm you've internalized the operating character + the §3 anti-drift discipline and have read
the M1 plan, then begin executing the plan — pausing only for genuine blockers or real decisions.
