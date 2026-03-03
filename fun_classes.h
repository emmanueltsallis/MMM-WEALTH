/*******************************************************************************
 * fun_classes.h - CLASS-Level Aggregation Module
 *
 * Phase 1: Functional distribution (Workers vs Capitalists)
 *
 * These equations compute class-level totals by summing over HOUSEHOLD
 * instances within each CLASS. Mirrors the SECTORS→FIRMS aggregation pattern.
 *
 * Naming convention: Household_X → Class_X (sums implied)
 *                    Averages use Class_Avg_X
 *
 * CLASS hierarchy:
 * - CLASS (class_id=0): Workers
 * - CLASS (class_id=1): Capitalists
 *
 * REQUIRES: LSD structure with CLASSES container and CLASS objects
 *           fun_households.h must be included before this file
 ******************************************************************************/

#ifndef FUN_CLASSES_H
#define FUN_CLASSES_H


/*============================================================================
 * COUNTS
 *============================================================================*/

EQUATION("Class_Household_Count")
/*
Total number of households in this class.
*/
RESULT(COUNT("HOUSEHOLD"))


EQUATION("Class_Employed_Count")
/*
Number of employed households in this class.
For workers: count those with Employment_Status > 0
For capitalists: always 0 (capitalists don't have employment status)
*/
v[0] = 0;
CYCLE(cur, "HOUSEHOLD")
{
    v[1] = VS(cur, "Household_Employment_Status");
    if(v[1] > 0)
        v[0]++;
}
RESULT(v[0])


EQUATION("Class_Unemployed_Count")
/*
Number of unemployed households in this class.
For workers: count those with Employment_Status == 0
For capitalists: always 0
*/
v[0] = 0;
CYCLE(cur, "HOUSEHOLD")
{
    v[1] = VS(cur, "Household_Employment_Status");
    if(v[1] == 0)
        v[0]++;
}
RESULT(v[0])


/*============================================================================
 * INCOME (Sums)
 *============================================================================*/

EQUATION("Class_Wage_Income")
/*
Total wage income for all households in this class.
*/
RESULT(SUM("Household_Wage_Income"))


EQUATION("Class_Profit_Income")
/*
Total profit income for all households in this class.
*/
RESULT(SUM("Household_Profit_Income"))


EQUATION("Class_Unemployment_Benefits")
/*
Total unemployment benefits for all households in this class.
*/
RESULT(SUM("Household_Unemployment_Benefits"))


EQUATION("Class_Nominal_Gross_Income")
/*
Total gross income for all households in this class.
*/
RESULT(SUM("Household_Nominal_Gross_Income"))


EQUATION("Class_Income_Taxation")
/*
Total income tax paid by all households in this class.
*/
RESULT(SUM("Household_Income_Taxation"))


EQUATION("Class_Nominal_Disposable_Income")
/*
Total disposable income for all households in this class.
*/
RESULT(SUM("Household_Nominal_Disposable_Income"))


EQUATION("Class_Real_Disposable_Income")
/*
Total real disposable income for all households in this class.
*/
RESULT(SUM("Household_Real_Disposable_Income"))


EQUATION("Class_Avg_Real_Income")
/*
Total average real income (smoothed) for all households in this class.
*/
RESULT(SUM("Household_Avg_Real_Income"))


EQUATION("Class_Avg_Nominal_Income")
/*
Total average nominal income (smoothed) for all households in this class.
*/
RESULT(SUM("Household_Avg_Nominal_Income"))


EQUATION("Class_Transfer_Received")
/*
Total transfers received by all households in this class.
*/
RESULT(SUM("Household_Transfer_Received"))


EQUATION("Class_Deposits_Return")
/*
Total interest income on deposits for all households in this class.
*/
RESULT(SUM("Household_Deposits_Return"))


/*============================================================================
 * CONSUMPTION (Sums)
 *============================================================================*/

EQUATION("Class_Real_Domestic_Consumption_Demand")
/*
Total real domestic consumption demand for all households in this class.
*/
RESULT(SUM("Household_Real_Domestic_Consumption_Demand"))


EQUATION("Class_Real_Desired_Imported_Consumption")
/*
Total real desired imported consumption for all households in this class.
*/
RESULT(SUM("Household_Real_Desired_Imported_Consumption"))


EQUATION("Class_Effective_Expenses")
/*
Total effective expenses for all households in this class.
*/
RESULT(SUM("Household_Effective_Expenses"))


EQUATION("Class_Real_Autonomous_Consumption")
/*
Total real autonomous consumption for all households in this class.
*/
RESULT(SUM("Household_Real_Autonomous_Consumption"))


EQUATION("Class_Real_Desired_Domestic_Consumption")
/*
Total real desired domestic consumption for all households in this class.
*/
RESULT(SUM("Household_Real_Desired_Domestic_Consumption"))


EQUATION("Class_Desired_Expenses")
/*
Total desired expenses for all households in this class.
*/
RESULT(SUM("Household_Desired_Expenses"))


EQUATION("Class_Effective_Real_Domestic_Consumption")
/*
Total effective real domestic consumption for all households in this class.
*/
RESULT(SUM("Household_Effective_Real_Domestic_Consumption"))


EQUATION("Class_Effective_Real_Imported_Consumption")
/*
Total effective real imported consumption for all households in this class.
*/
RESULT(SUM("Household_Effective_Real_Imported_Consumption"))


EQUATION("Class_Retained_Deposits")
/*
Total retained deposits (precautionary) for all households in this class.
*/
RESULT(SUM("Household_Retained_Deposits"))


EQUATION("Class_Internal_Funds")
/*
Total internal funds available for all households in this class.
*/
RESULT(SUM("Household_Internal_Funds"))


EQUATION("Class_Maximum_Expenses")
/*
Total maximum possible expenses for all households in this class.
*/
RESULT(SUM("Household_Maximum_Expenses"))


EQUATION("Class_Asset_Purchases")
/*
Total financial asset purchases for all households in this class.
*/
RESULT(SUM("Household_Asset_Purchases"))


EQUATION("Class_Avg_Imports_Share")
/*
Average import share across households in this class.
*/
RESULT(AVE("Household_Imports_Share"))


/*============================================================================
 * FINANCIAL STOCKS (Sums)
 *============================================================================*/

EQUATION("Class_Stock_Deposits")
/*
Total deposits held by all households in this class.
*/
RESULT(SUM("Household_Stock_Deposits"))


EQUATION("Class_Stock_Loans")
/*
Total loans owed by all households in this class.
*/
RESULT(SUM("Household_Stock_Loans"))


EQUATION("Class_Financial_Assets")
/*
Total financial assets held by all households in this class.
*/
RESULT(SUM("Household_Financial_Assets"))


EQUATION("Class_Net_Wealth")
/*
Total net wealth for all households in this class.
*/
RESULT(SUM("Household_Net_Wealth"))


EQUATION("Class_Savings")
/*
Total savings for all households in this class.
*/
RESULT(SUM("Household_Savings"))


/*============================================================================
 * FINANCIAL FLOWS (Sums)
 *============================================================================*/

EQUATION("Class_Interest_Payment")
/*
Total interest payments by all households in this class.
*/
RESULT(SUM("Household_Interest_Payment"))


EQUATION("Class_Debt_Payment")
/*
Total debt payments by all households in this class.
*/
RESULT(SUM("Household_Debt_Payment"))


EQUATION("Class_Demand_Loans")
/*
Total loan demand by all households in this class.
*/
RESULT(SUM("Household_Demand_Loans"))


EQUATION("Class_Effective_Loans")
/*
Total effective (granted) loans to all households in this class.
*/
RESULT(SUM("Household_Effective_Loans"))


EQUATION("Class_Max_Loans")
/*
Total maximum allowed loans for all households in this class.
*/
RESULT(SUM("Household_Max_Loans"))


EQUATION("Class_Financial_Obligations")
/*
Total financial obligations for all households in this class.
*/
RESULT(SUM("Household_Financial_Obligations"))


EQUATION("Class_Avg_Interest_Rate")
/*
Average interest rate across households in this class.
*/
RESULT(AVE("Household_Interest_Rate"))


EQUATION("Class_Avg_Max_Debt_Rate")
/*
Average maximum debt rate across households in this class.
*/
RESULT(AVE("Household_Max_Debt_Rate"))


/*============================================================================
 * WEALTH TAX (Sums)
 *============================================================================*/

EQUATION("Class_Wealth_Tax_Owed")
/*
Total wealth tax owed by all households in this class.
*/
RESULT(SUM("Household_Wealth_Tax_Owed"))


EQUATION("Class_Wealth_Tax_Payment")
/*
Total wealth tax paid by all households in this class.
*/
RESULT(SUM("Household_Wealth_Tax_Payment"))


EQUATION("Class_Wealth_Tax_From_Deposits")
/*
Total wealth tax paid from deposits by all households in this class.
*/
RESULT(SUM("Household_Wealth_Tax_From_Deposits"))


EQUATION("Class_Wealth_Tax_From_Assets")
/*
Total wealth tax paid from asset liquidation by all households in this class.
*/
RESULT(SUM("Household_Wealth_Tax_From_Assets"))


EQUATION("Class_Wealth_Tax_From_Borrowing")
/*
Total wealth tax paid via borrowing by all households in this class.
*/
RESULT(SUM("Household_Wealth_Tax_From_Borrowing"))


EQUATION("Class_Wealth_Tax_From_Buffer")
/*
Total wealth tax paid from liquidity buffer by all households in this class.
*/
RESULT(SUM("Household_Wealth_Tax_From_Buffer"))


/*============================================================================
 * EVASION & CAPITAL FLIGHT (Sums)
 *============================================================================*/

EQUATION("Class_Deposits_Offshore")
/*
Total offshore deposits for all households in this class.
*/
RESULT(SUM("Household_Deposits_Offshore"))


EQUATION("Class_Deposits_Domestic")
/*
Total domestic deposits for all households in this class.
*/
RESULT(SUM("Household_Deposits_Domestic"))


EQUATION("Class_Assets_Undeclared")
/*
Total undeclared financial assets for all households in this class.
*/
RESULT(SUM("Household_Assets_Undeclared"))


EQUATION("Class_Assets_Declared")
/*
Total declared financial assets for all households in this class.
*/
RESULT(SUM("Household_Assets_Declared"))


EQUATION("Class_Repatriated_Deposits")
/*
Total repatriated deposits for all households in this class.
*/
RESULT(SUM("Household_Repatriated_Deposits"))


EQUATION("Class_Asset_Penalty")
/*
Total asset evasion penalties for all households in this class.
*/
RESULT(SUM("Household_Asset_Penalty"))


EQUATION("Class_Offshore_Penalty")
/*
Total offshore penalties for all households in this class.
*/
RESULT(SUM("Household_Offshore_Penalty"))


EQUATION("Class_Audited_Count")
/*
Number of audited households in this class.
*/
RESULT(SUM("Household_Is_Audited"))


EQUATION("Class_Flight_Count")
/*
Number of households engaging in capital flight in this class.
*/
RESULT(SUM("Household_Decision_Flight"))


EQUATION("Class_Evasion_Count")
/*
Number of households engaging in asset evasion in this class.
*/
RESULT(SUM("Household_Decision_Evasion"))


/*============================================================================
 * AVERAGES
 *============================================================================*/

EQUATION("Class_Avg_Disposable_Income")
/*
Average disposable income per household in this class.
*/
v[0] = V("Class_Nominal_Disposable_Income");
v[1] = V("Class_Household_Count");
RESULT((v[1] > 0) ? v[0] / v[1] : 0)


EQUATION("Class_Avg_Net_Wealth")
/*
Average net wealth per household in this class.
*/
v[0] = V("Class_Net_Wealth");
v[1] = V("Class_Household_Count");
RESULT((v[1] > 0) ? v[0] / v[1] : 0)


EQUATION("Class_Avg_Propensity_to_Spend")
/*
Average propensity to spend across households in this class.
*/
RESULT(AVE("Household_Propensity_to_Spend"))


EQUATION("Class_Avg_Savings_Rate")
/*
Average savings rate across households in this class.
*/
RESULT(AVE("Household_Savings_Rate"))


EQUATION("Class_Avg_Debt_Rate")
/*
Average debt rate across households in this class.
*/
RESULT(AVE("Household_Debt_Rate"))


EQUATION("Class_Avg_Reference_Income")
/*
Average reference income (habit stock) across households in this class.
*/
RESULT(AVE("Household_Reference_Income"))


/*============================================================================
 * SHARES (relative to country total)
 *============================================================================*/

EQUATION("Class_Income_Share")
/*
This class's share of total household disposable income.
NOTE: Explicitly sums both classes because SUMS(country, ...) only finds first CLASSES.
*/
v[0] = V("Class_Nominal_Disposable_Income");
v[1] = VS(working_class, "Class_Nominal_Disposable_Income") + VS(capitalist_class, "Class_Nominal_Disposable_Income");
RESULT((v[1] > 0) ? v[0] / v[1] : 0)


EQUATION("Class_Wealth_Share")
/*
This class's share of total household net wealth.
NOTE: Explicitly sums both classes because SUMS(country, ...) only finds first CLASSES.
*/
v[0] = V("Class_Net_Wealth");
v[1] = VS(working_class, "Class_Net_Wealth") + VS(capitalist_class, "Class_Net_Wealth");
RESULT((v[1] > 0) ? v[0] / v[1] : 0)


EQUATION("Class_Consumption_Share")
/*
This class's share of total household consumption.
NOTE: Explicitly sums both classes because SUMS(country, ...) only finds first CLASSES.
*/
v[0] = V("Class_Effective_Expenses");
v[1] = VS(working_class, "Class_Effective_Expenses") + VS(capitalist_class, "Class_Effective_Expenses");
RESULT((v[1] > 0) ? v[0] / v[1] : 0)


#endif // FUN_CLASSES_H
