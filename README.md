## **MMM-WEALTH: Multisectoral Micro-Macro Model with Wealth Tax & Evasion Dynamics**

Built on top of the V3.6 (Finance Augmented) version, this model extends the original framework by incorporating wealth taxation and evasion dynamics.

## WHAT'S NEW?
### Wealth Taxation & Evasion Dynamics Module

This version introduces a comprehensive module for Wealth Taxation, incorporating endogenous Tax Evasion and Capital Flow dynamics. Key features include:

*   **Class Structure:** The model implements a derives from a 3-class structure (class A, B and C) to a 6-class structure following the Paris School of Economics literature and World Inequality Database (WID) methodology:
    *   **Bottom 50% (P0-P50):** The lower, working class
    *   **Middle 40% (P50-P90):** The middle class
    *   **Next 9% (P90-P99):** The upper middle class
    *   **Next 0.9% (P99-P99.9):** The rich
    *   **Next 0.09% (P99.9-P99.99):** The super rich
    *   **Top 0.01% (P99.99-P100):** The ultra rich
This structure enables more granular analysis of wealth dynamics and tax incidence across different segments of the wealth distribution, while maintaining consistency with widely used inequality metrics.

*   **Wealth Heterogeneity:** The model now allows for negative net wealth (debt) at the class level. It assumes an intra-class wealth distribution (e.g., Pareto-Tsallis, or q-Exponential) to estimate the proportion of households liable for wealth tax based on a specified threshold.

*   **Wealth Taxation:** A configurable tax is implemented on household net wealth exceeding a defined threshold. Potential tax revenue is calculated based on the estimated taxpayer base and their average wealth above the threshold.

*   **Endogenous Tax Evasion Decision:** Income classes dynamically decide the proportion of domestic wealth to evade via capital flight. This decision is based on a comparison of expected payoffs (following the Evolutionary Game Theory), considering:
    *   **Compliance Payoff:** Domestic interest rates and the effective wealth tax burden.
    *   **Evasion Payoff:** Foreign interest rates, *expected* exchange rate changes, and the *expected* probability of detection and associated penalties.
    
*   **Endogenous Capital Repatriation Decision:** Classes with existing external wealth decide the proportion to repatriate based on comparing expected payoffs:
    *   **Repatriation Payoff:** Domestic interest rates and potential taxes on repatriated wealth.
    *   **Keep Abroad Payoff:** Foreign interest rates and *realized* exchange rate changes on the domestic currency value of external assets.

*   **Capital Flight & Repatriation Dynamics:** These are modeled as flows that alter domestic and external wealth stocks (tracked in domestic currency), ensuring stock-flow consistency (SFC).

*   **Evolutionary Government Enforcement Learning Process:** The government's audit rate (probability of detecting capital flight) evolves through a learning mechanism that adapts based on past and observed evasion patterns. This process incorporates:
    * Historical evasion rate trends
    * Success rate of previous audits
    * Resource constraints and enforcement costs
    * Feedback loops between enforcement intensity and evasion behavior

*   **Adaptive Expectations:** Income classes form expectations about the government's audit rate adaptively, based on past observed rates and their specific expectation formation parameters.

*   **Wealth Penalties:** If capital flight is detected (stochastically), a penalty is levied on the evaded amount. This contributes to government revenue but reduces the amount successfully transferred abroad.

*   **Exchange Rate & Balance of Payments Effects:** Exchange rate dynamics and balance of payments effects are explicitly modeled. These influence the domestic currency value of external assets, the payoff calculations for evasion and repatriation decisions, and the overall macroeconomic stability through their impact on foreign reserves, current account, and capital account balances.

*   **Capital Controls:** The model includes a switch to simulate two scenarios: (1) an open economy with unrestricted capital flows, and (2) a closed economy with capital controls. Capital controls serve as a policy tool to prevent tax evasion through capital flight by restricting the movement of wealth across borders.


## ORIGINAL MMM MODEL (v. 3.6)

Introduction of Banking System
* N number of Bank Agents
* Basileia-type Regulatory Rule
* Financial Fragility Sensitivity

Endogeneization of Firm's and Classes Financial Decisions
* Desired Debt Rate
* Liquidity Preference

Introduction of Financial Costs to Firms

Introduction of Interest Rate Structure
* Basic Interest Rate Following Possible Monetary Policy Rules
* Deposits Spread
* Short-Term Loans Spread
* Long-Term Loans Spread
* Individual Risk-Premium

Introduction of Some Analysis Variables
* Firm's Financial Positions (Hedge, Speculative, Ponzi)
* Bankrupt events counter
* Bank and Financial Sector Variables

For more information on previous versions: <https://thttnn.github.io/MMM_CORE/>

### BANKING SYSTEM:
The banking system is composed by a pre-determined number of banks, which collect deposits from firms and income classes and provide loans to them. 
The number of banks is fixed and they all start equally.
Firm-bank relationships are determined randomly at the beginning of the simulation run, but after, the probability is weighted by bank's market-share.
Firm-bank relationships are fixed for the simulation run.
Income classes deposits, interest and loans are distributed to banks by market-share.
Banks have a limit to total credit based on basileia-type regulatory rule, net worth and individual financial fragility sensitivities.
However, credit rationing affects bank's competitiveness negativelly, as well as defaulted loans. 

### INTEREST RATE STRUCTURE:
Banks set deposits and short-term interest rates by simply applying a markup or markdown on basis interest rate.
Long-term interest rates are adjusted by bank's market-share. Therefore, the higher the concentration, higher is the average long-term rate.
Basic Interest Rate depends on the monetary policy rule.

### FIRM'S FINANCIAL DECISIONS:
Firms can adjust (or not) desired debt rate and liquidity preference (as a share of capital) based on net profits growth.
Firms can adjust extra amortization (or not) based on current and desired debt rate.
Firms can incorporate (or not) financial costs to unit variable cost if current debt rate is higher than desired.
Income classes can also adjust desired debt rate and liquidity preference based on net income growth.


