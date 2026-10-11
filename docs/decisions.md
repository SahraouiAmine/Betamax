## 2026-10-10: No runtime overflow checks in Rational
**Context:** int64_t, used in Rational, overflows beyond 2⁶³ - 1. The addition operator can introduce numbers as large as 2M^2 where M is the maximum component. This must be less than 2⁶³ - 1. How do we avoid this?

**Decision:** We cap generated problem components to well below 32 bits. Correct answers will always be < 99,999.

**Why:** We want something that a human can hold and manipulate in their head anyways, so it serves double purpose: no overflow, and human-playable/speed-game friendly.

**Consequences:** The generator has a extra work to do: it must keep values small. The biggest risk comes from chained sums with coprime factors, as they can't be reduced along the way by Rational. 

**Verification:** Two verification layers: UBSan and with property tests on generators. We also limit user input to 99,999.
