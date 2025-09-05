
# Renderer Optimizations before class refactoring
For **branch**: `renderer-optimization-1`

## Priorities

1. **High**: Raycasting algorithm, sin/cos cache instead of evaluation
2. **Medium**: Redrawing, consts
3. **Low**: function/method subdivision, structure improvements (..refactoring moment)

---

## Problem review

1. Non-efficient raycasting
`stepSize=0.01f` makes ~1600 iterations of one ray
Better to use DDA (Digital Difference Analyzer) algorithm

2. Redundancy of evaluations in `render()`
Optimize trigonometry evalf

3. Slow screen redrawing
Batch drawing symbols to batches organized by the same color

4. Data structures optimization
Use arrays `array` instead of `vector` for critical paths
Optimization of `map` access

5. Condition simplification and etc

### Solved problems
- [ ] Raycasting
- [ ] Optimized trigonometry
- [ ] Moved evals to consts, etc
- [ ] Redrawing
- [ ] Data stucrutures changed
- [ ] Conditions simplified (*additionally*)

