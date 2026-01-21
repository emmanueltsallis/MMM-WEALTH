# MMM-WEALTH: A Post-Keynesian-Evolutionary-Complexity Stock-Flow Consistent Agent-Based Model with Wealth Disaggregation

## Executive Summary

This document details the **MMM-WEALTH** household-disaggregated agent-based model, transformed from the UFRJ Micro-Macro Multisectoral Model (MMM v3.6) from Vianna (2021). The transformation preserves stock-flow consistency while enabling endogenous wealth inequality dynamics within a heterodox macroeconomic framework.

### Core Transformation

| Dimension | UFRJ Reference (Class-Based) | MMM-WEALTH (Household-Based) |
|-----------|------------------------------|------------------------------|
| **Agent granularity** | 3 income classes (A, B, C) | N individual households (100-10,000+) |
| **Behavioral heterogeneity** | Class-average parameters | Individual parameters via distributions |
| **Income distribution** | Exogenous parametric shares | Endogenous from labor market + q-exponential |
| **Inequality scope** | Between-class (Kaleckian functional) | Within-type + between-type (Kaleckian + Piketty) |
| **Consumption function** | Linear MPC by class | Duesenberry ratchet with habit formation |
| **Employment** | Implicit in wage shares | Explicit MARKOV transitions (capacity-driven) |
| **Wealth accumulation** | Aggregated deposits/loans | Individual balance sheets with financial assets |

### Theoretical Contributions

The MMM-WEALTH model operationalizes:
- **Duesenberry's relative income hypothesis** via interpersonal and intertemporal consumption ratchets
- **Kaleckian distribution theory** through endogenous profit shares and capacity-driven employment
- **Post-Keynesian financial dynamics** via Minsky-style liquidity preference and debt constraints
- **Piketty's r > g mechanism** through financialization and asset shortage dynamics
- **Complexity emergence** where macro inequality patterns emerge from micro household interactions

## 1. Theoretical Foundation

### 1.1 Paradigmatic Foundations

Both models are grounded in **heterodox macroeconomics**, rejecting neoclassical optimization-equilibrium methodology:

| Paradigm | Core Tenets | Model Implementation |
|----------|-------------|----------------------|
| **Post-Keynesian** | Effective demand primacy; fundamental uncertainty | Demand-driven production; liquidity preference |
| **Kaleckian** | Profit-wage conflict; markup pricing | Class/type income shares; cost-plus pricing |
| **Evolutionary** | Bounded rationality; routines and learning | Firm heterogeneity; R&D and imitation |
| **Complexity** | Emergence; path dependence | Agent interactions generate macro patterns |
| **Stock-Flow Consistent** | Balance sheet coherence; no black holes | Complete sectoral accounts verified continuously |

### 1.2 Ontological Position

The MMM family adopts an **open-systems ontology** where:
- **Causation is tendential**, not deterministic
- **Agents are boundedly rational**, following adaptive rules
- **Time is historical**, with path dependence fundamental
- **Equilibrium is rejected** as organizing concept

### 1.3 Post-Keynesian Justification for Disaggregation

The disaggregation addresses key limitations of aggregate approaches:

**Methodological Concerns:**
- **Representative Agent Fallacy**: Classes assumed identical behavior within brackets
- **Distributional Blindness**: Within-class inequality dynamics ignored
- **Behavioral Homogeneity**: Identical preferences and constraints within classes

**Kaleckian Motivation:**
- Worker-capitalist distinction preserved but enriched with within-class heterogeneity
- Personal income/wealth distribution emerges from labor market + profit distribution
- Capacity utilization feedback via MARKOV employment dynamics

## 2. Model Architecture

### 2.1 Object Hierarchy

```
COUNTRY
├── SECTORS (3: Consumption, Capital, Input)
│   └── FIRMS (5-20 per sector)
├── HOUSEHOLDS (N individual agents, 100-10,000+)
│   └── HOUSEHOLD_LOANS (dynamically created)
├── GOVERNMENT (enhanced with wealth taxation)
├── FINANCIAL + BANKS (enhanced loan allocation)
├── CENTRAL_BANK
└── EXTERNAL_SECTOR
```

### 2.2 Household Agent Structure

Each household is characterized by:

**Identity Parameters:**
- `household_id`: Unique identifier
- `household_type`: 0 (worker) or 1 (capitalist)

**Behavioral Parameters (heterogeneous):**
- `household_skill`: Labor productivity (log-normal, E[X]=1.0)
- `household_propensity_to_spend`: MPC (type-dependent)
- `household_liquidity_preference`: Savings preference (normal distribution)
- `household_habit_persistence`: Consumption ratchet strength (β)
- `household_profit_share`: Share of profits (q-exponential for capitalists)

## 3. Key Economic Mechanisms

### 3.1 Employment Dynamics: Kaleckian Labor Market

**Gap-driven MARKOV transitions** replace implicit wage shares:

```
Gap = Country_Capacity_Utilization - target_utilization

P_hire = base_hire × (1 + hiring_speed × max(0, Gap)) × skill_boost
P_fire = base_fire × (1 + firing_speed × max(0, -Gap)) / skill_protect
```

Employment responds to demand conditions following Kaleckian/Post-Keynesian principles.

### 3.2 Income Distribution: Kalecki-Piketty Bridge

**Wage Income (workers only):**
```
Homogeneous: Household_Wage = Total_Wages / N_employed
Heterogeneous: Household_Wage = Total_Wages × (skill_i / Σskill_employed)
```

**Profit Income (capitalists only):**
```
Household_Profit_Income = Total_Profits × household_profit_share_i
where profit shares ~ Q-exponential(q=1.5, λ=1.0), normalized to sum to 1.0
```

### 3.3 Consumption: Duesenberry Ratchet

**Reference Income Evolution:**
```
If Y_t > R_{t-1}: R_t = R_{t-1} + λ⁺ × (Y_t - R_{t-1})   [fast inflation]
If Y_t ≤ R_{t-1}: R_t = R_{t-1} + λ⁻ × (Y_t - R_{t-1})   [slow deflation]

Typical values: λ⁺ ≈ 0.8 (fast habituation), λ⁻ ≈ 0.3 (slow de-habituation)
```

**Propensity to Spend:**
```
Base MPC = f(income_percentile)  [generalized logistic, decreasing]
Ratchet Boost = β × max(0, 1 - Y/R)  [penalty when below reference]
Final MPC = Base + Ratchet  [can exceed 1.0 for distressed households]
```

### 3.4 Financial Infrastructure: Minsky-Kalecki Dynamics

**Balance Sheet:**
```
Household_Net_Wealth = Deposits + Financial_Assets - Loans
```

**Budget Constraint:**
```
Household_Internal_Funds = Deposits_{t-1} + Income - Retained_Liquidity - Obligations
Household_Maximum_Expenses = min(Desired, Internal_Funds + Effective_Loans)
```

## 4. Wealth Accumulation and Concentration

### 4.1 Accumulation Channels

**Primary (Savings → Deposits):**
- Workers: High MPC (0.85-0.95) → small deposit accumulation
- Capitalists: Low MPC (0.4-0.6) → substantial accumulation

**Secondary (Deposits → Financial Assets, capitalists only):**
```
Household_Asset_Purchases = Savings × (1 - liquidity_preference)
```

**Tertiary (Capital Gains):**
```
Capital_Gains = Financial_Assets_{t-1} × Financial_Asset_Inflation_Rate
```

### 4.2 r > g Mechanism (Piketty)

The model operationalizes Piketty's central inequality:
```
r_effective = (Profit_Income + Capital_Gains + Interest) / Net_Wealth
g = Country_Annual_Real_Growth

When r > g (sustained): Wealth concentrates toward capitalists
```

### 4.3 Financialization and Secular Stagnation

```
Savings_desired > Investment_opportunities
    → Asset shortage → Asset price inflation
    → r_assets > r_productive → Shift to financial assets
    → Real investment falls → Growth slows (g ↓)
    → r > g intensifies → Wealth concentration accelerates
```

## 5. Wealth Taxation Module

### 5.1 Tax Liability

```
Household_Wealth_Tax_Owed = max(0, (Net_Wealth - threshold) × rate)
```

### 5.2 Two-Stage Behavioral Payment

**Stage 1: Liquidity Decision**
```
Available_for_Tax = Deposits × (1 - liquidity_preference)
From_Deposits = min(Tax_Owed, Available_for_Tax)
```

**Stage 2: Debt vs Asset Decision**
```
Debt_Headroom = (Max_Debt_Rate - Current_Debt_Rate) / Max_Debt_Rate
From_Borrowing = Remainder × Debt_Headroom
From_Assets = Remainder × (1 - Debt_Headroom)
```

## 6. Aggregation and Validation

### 6.1 Stock-Flow Consistency

| Identity | Formula | Status |
|----------|---------|--------|
| GDP (Production) | `GDP = Wages + Profits + Indirect_Taxes` | ✓ Preserved |
| GDP (Demand) | `GDP = C + I + G + X - M` | ✓ Preserved |
| Wage distribution | `Σ(Household_Wage_Income) = Total_Wages` | ✓ New identity |
| Profit distribution | `Σ(Household_Profit_Income) = Total_Profits` | ✓ New identity |
| Wealth tax | `Government_Revenue = Σ(Household_Payments)` | ✓ New identity |

### 6.2 Distributional Realism

| Metric | Expected Range | Empirical Benchmark |
|--------|----------------|---------------------|
| Income Gini | 0.35-0.55 | OECD: 0.30-0.50 |
| Wealth Gini | 0.70-0.90 | US: 0.85, Europe: 0.70 |
| Unemployment | 3-12% | NAIRU: 4-6% |
| Wage share | 50-70% | Historical: 55-65% |
| Top 10% wealth | 50-80% | US: 70%, Europe: 50% |

## 7. Policy Analysis Capabilities

### 7.1 Distributional Policy Analysis

| Experiment | MMM-WEALTH Capability |
|------------|----------------------|
| Progressive wealth tax | Threshold + rate on individual wealth |
| Universal basic income | Transfer to all below threshold |
| Employment guarantee | MARKOV hiring floor |
| Debt relief programs | Individual debt forgiveness |
| Skill-based policies | Training effects on hiring probability |

### 7.2 New Analytical Capabilities

**Enabled by disaggregation:**
- Wealth tax incidence by wealth decile
- Skill-biased unemployment during recessions
- Consumption collapse scenarios (coordinated de-leveraging)
- Household leverage distributions and debt-at-risk analysis

## 8. Emergent Dynamics

The model generates **emergent macro phenomena** from micro interactions:

**Emergent Inequality:**
- Gini coefficients are *not* calibrated—they emerge from employment shocks × skill distributions × profit shares × saving behavior

**Emergent Financial Instability (Minsky):**
- Aggregate leverage emerges from individual borrowing decisions
- Credit cycles: confidence → leverage → fragility → crisis

**Hysteresis and Path Dependence:**
- Employment/debt histories constrain future behavior
- Wealth concentration compounds over time (r > g)

## 9. Technical Implementation

| Category | Parameters | Purpose |
|----------|------------|---------|
| Heterogeneity | 8 | Skill, habit, liquidity distributions |
| Employment | 5 | MARKOV transition parameters |
| Consumption | 4 | Duesenberry curve shape |
| Wealth tax | 2 | Rate and threshold |

## 10. Conclusions

### Methodological Innovation

MMM-WEALTH integrates three streams:
1. **SFC Models** (Godley-Lavoie): Complete balance sheet tracking
2. **Agent-Based Models** (Dosi-Fagiolo-Roventini): Individual heterogeneity
3. **Inequality Economics** (Piketty-Saez): Wealth distribution tracking

### Unique Contribution

Among the first models to combine:
- Household-level SFC accounting (not just firm-level)
- Endogenous personal distribution (not just functional)
- Behavioral wealth tax responses
- Duesenberry consumption dynamics in an ABM framework

### Limitations and Extensions

**Current Limitations:**
- No intergenerational transfers (bequests)
- No housing/real estate sector
- Firms not explicitly household-owned

**Planned Extensions:**
- Tax evasion and capital flight
- Advanced financialization
- Intergenerational wealth transmission

---

*Model comparison based on UFRJ MMM v3.6 (reference) and MMM-WEALTH through Stage 7 (wealth taxation). Methodological framework: Kaleckian-Post-Keynesian demand-led growth with SFC accounting and evolutionary micro-dynamics.*
