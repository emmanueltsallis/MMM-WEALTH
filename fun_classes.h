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


/*============================================================================
 * SHARES (relative to country total)
 *============================================================================*/

EQUATION("Class_Income_Share")
/*
This class's share of total household disposable income.
*/
v[0] = V("Class_Nominal_Disposable_Income");
v[1] = SUMS(households, "Household_Nominal_Disposable_Income");
RESULT((v[1] > 0) ? v[0] / v[1] : 0)


EQUATION("Class_Wealth_Share")
/*
This class's share of total household net wealth.
*/
v[0] = V("Class_Net_Wealth");
v[1] = SUMS(households, "Household_Net_Wealth");
RESULT((v[1] > 0) ? v[0] / v[1] : 0)


EQUATION("Class_Consumption_Share")
/*
This class's share of total household consumption.
*/
v[0] = V("Class_Effective_Expenses");
v[1] = SUMS(households, "Household_Effective_Expenses");
RESULT((v[1] > 0) ? v[0] / v[1] : 0)


#endif // FUN_CLASSES_H
