/*****MACRO VARIABLES******/


EQUATION("Loans_Distribution_Firms");
/*
Distributed effective loans to firms if there is credit rationing
This variable is very important
It is located in the macro level
There are some underlying hypothesis:
1-Income classes are never credit rationed and receive loans first.
2-A bank has a total amount of loans it can provide. After discounting the amount for the income classes, it distribute proportionally to each sector
3-Within each sector, it provides in a order of debt rate. High indebtedness firms migh not receive loans.
*/

v[0]=SUM("Firm_Demand_Loans");						//total demand of firm loans
v[12]=SUMS(working_class, "Household_Demand_Loans") + SUMS(capitalist_class, "Household_Demand_Loans");  // Phase E: sum both classes

CYCLE(cur, "BANKS")
{
	v[1]=VS(cur, "Bank_Effective_Loans");
	v[2]=VS(cur, "bank_id");
	v[13]=VS(cur, "Bank_Market_Share");
	v[14]=v[13]*v[12];
	v[10]=0;
	CYCLES(root, cur1, "SECTORS")
	{
		v[3]=SUMS(cur1, "Firm_Demand_Loans");			//sector demand of loans
		if(v[0]!=0)
			v[4]=v[3]/v[0];								//sector share of demand
		else
			v[4]=0;
		
		v[5]=max(0,(v[1]-v[14])*v[4]);
		v[11]=V("switch_creditworthness");
				
			v[9]=0;
			if(v[11]==1)
				SORTS(root, "FIRMS", "Firm_Avg_Debt_Rate", "UP");
			if(v[11]==2)
				SORTS(root, "FIRMS", "firm_date_birth", "UP");
			if(v[11]==3)
				SORTS(root, "FIRMS", "firm_date_birth", "DOWN");
			CYCLES(cur1, cur2, "FIRMS")
			{
				v[6]=VS(cur2, "firm_bank");
					if (v[6]==v[2])
					{
						v[7]=VS(cur2, "Firm_Demand_Loans");
						if (v[5]>=v[7])
							v[8]=v[7];
						else
							v[8]=max(0, v[5]);
						v[5]=v[5]-v[8];
						WRITES(cur2, "firm_effective_loans", v[8]);
						v[9]=v[9]+1;
					}
					else
					{
						v[5]=v[5];
						v[9]=v[9];
					}		
			}

	v[10]=v[10]+v[9];
	}
WRITES(cur, "Bank_Number_Clients", v[10]);
}	
RESULT(0)

	
EQUATION("Country_Domestic_Intermediate_Demand")
/*
Calculates the domestic demand for inputs.
Must be called by the sectors.
*/
	v[2]=0;                                                      		//initializes the value for thr CYCLE
	CYCLE(cur, "SECTORS")                                        		//CYCLE trought all sectors
		v[2]=v[2]+SUMS(cur, "Firm_Input_Demand_Next_Period");           //sums up the demand for imputs of all setors
	v[0]=VS(input, "Sector_Avg_Price");
	v[1]=V("Government_Effective_Inputs");
	v[5]=v[0]!=0? v[1]/v[0] : 0;
	v[6]=v[2]+v[5];
RESULT(v[6])


EQUATION("Country_Domestic_Consumption_Demand")
/*
Stage 4.7 SWITCH #1: Household-driven consumption demand.
Calculates the domestic demand for consumption goods.
Must be called by the sector.

SWITCHED: From SUM("Class_Real_Domestic_Consumption_Demand")
          To SUMS(households, "Household_Real_Domestic_Consumption_Demand")
*/
	v[0]=SUMS(working_class, "Household_Real_Domestic_Consumption_Demand") + SUMS(capitalist_class, "Household_Real_Domestic_Consumption_Demand");
	v[1]=VS(consumption, "Sector_Avg_Price");
	v[2]=V("Government_Effective_Consumption");
	v[3]= v[1]!=0? v[2]/v[1] : 0;
	v[4]=v[0]+v[3];
RESULT(v[4])


EQUATION("Country_Domestic_Capital_Goods_Demand")
/*
The demand for capital goods is calculated by summing up the demand for capital goods from all sectors with government spending on investment.
Must be called by the sectors.
*/
	v[1]=0;                                                 			//initializes the CYCLE
	CYCLE(cur, "SECTORS")                                   			//CYCLE trought the sectors
	{	
		v[10]=SUMLS(cur, "Firm_Demand_Capital_Goods_Expansion",1);
		v[11]=SUMLS(cur, "Firm_Demand_Capital_Goods_Replacement",1);
		v[1]=v[1]+v[10]+v[11];                                       	//sums up all firm's capital goods demand
	}
	v[4]=VS(capital, "Sector_Avg_Price");
	v[5]=V("Government_Effective_Investment");
	v[6]= v[4]!=0? v[5]/v[4] : 0;
	v[7]=v[1]+v[6];
RESULT(v[7])


EQUATION("Country_Capital_Goods_Price")
RESULT(VS(capital, "Sector_Avg_Price"))


EQUATION("Country_Price_Index")
/*
Average Price of all sector. GDP deflator
*/
	v[0]=WHTAVE("Sector_Sales","Sector_Avg_Price");
	v[1]=SUM("Sector_Sales");
	v[2]= v[1]!=0? v[0]/v[1] : CURRENT;
RESULT(v[2])


EQUATION("Country_Consumer_Price_Index")
/*
Average Price of the consumption goods sector.
Stage 4.7: Uses household import shares weighted by LAGGED income share.
Uses lagged income to avoid circular dependency (same as original Class_Income_Share).

PERFORMANCE OPTIMIZATION: Single-pass computation (was 2 passes, now 1).
Accumulates both total_income and weighted_import_sum simultaneously.
Reduces O(2N) to O(N) - 25% overall household scan reduction.
*/
	v[0]=VS(consumption, "Sector_Avg_Price");
	v[1]=VS(consumption, "Sector_External_Price");
	v[2]=VS(external, "Country_Exchange_Rate");

	// SINGLE PASS: Compute total lagged income AND weighted import sum together
	v[10]=0;  // total_lagged_income = Σ(income_i)
	v[11]=0;  // weighted_import_sum = Σ(import_share_i × income_i)

	CYCLE(cur1, "CLASSES")
	{
	CYCLES(cur1, cur, "HOUSEHOLD")
	{
		v[5]=VS(cur, "Household_Imports_Share");
		v[6]=VLS(cur, "Household_Nominal_Disposable_Income", 1);
		v[10]+=v[6];           // accumulate total income
		v[11]+=v[5]*v[6];      // accumulate weighted import (import_share × income)
	}
	}

	// weighted_avg_import_share = Σ(m_i × y_i) / Σ(y_i)
	v[3]=(v[10] > 0.01) ? v[11]/v[10] : 0;

	v[4]=v[0]*(1-v[3])+v[1]*v[2]*v[3];
RESULT(v[4])



EQUATION("Country_Annual_Inflation")
/*
Annual growth of the overall price index.
Uses support function
*/
RESULT(LAG_GROWTH(p, "Country_Price_Index", V("annual_frequency"), 1))


EQUATION("Country_Annual_CPI_Inflation")
/*
Annual growth of the consumer price index
Uses support function
*/
RESULT(LAG_GROWTH(p, "Country_Consumer_Price_Index", V("annual_frequency"), 1))


EQUATION("Country_Distributed_Profits")
/*
Total amount of distributed profits by the firms. Will be used to determine the income of the income classes.
*/
	v[0]=0;                                            		//initializes the CYCLE
	CYCLE(cur, "SECTORS")                              		//CYCLE trought all sectors
		v[0]=v[0]+SUMS(cur, "Firm_Distributed_Profits");    //sums up the value of distributed profits of all sectors
	v[3]=V("Financial_Sector_Distributed_Profits");
	v[4]=v[0]+v[3];
RESULT(v[4])


EQUATION("Country_Total_Profits")
/*
Total Surplus of the Economy. Is the sum of all firms net profits. Will be used to calculate GDP
*/
	v[0]=0;                                                    		//initializes the CYCLE
	CYCLE(cur, "SECTORS")                                      		//CYCLE trought all sectors
		v[0]=v[0]+SUMS(cur, "Firm_Net_Profits");                    //sums up the surplus of all sectors
	v[3]=V("Financial_Sector_Profits");
	v[4]=v[0]+v[3];
RESULT(v[4])


EQUATION("Country_Total_Wages")
/*
The total wage is calculated by the sum of the wages paid by the sectors with government wages. The wage per unit of production is a predetermined parameter, and the total salary is calculated by multiplying this unit wage by the actual production of each sector.
*/
	v[0]=0;                                                    		//initializes the CYCLE
	CYCLE(cur, "SECTORS")                                      		//CYCLE trought all sectors
	{
		v[1]=0;                                                  	//initializes the second CYCLE
		CYCLES(cur, cur1, "FIRMS")                               	//CYCLE trought all firms of the sector
		{
			v[2]=VS(cur1, "Firm_Wage");                             //firm's wage
			v[3]=VS(cur1, "Firm_Effective_Production");             //firm's effective production
			v[4]=VS(cur1, "Firm_Avg_Productivity");            		//firm's productivity in the last period
			v[5]=VS(cur1, "Firm_RND_Expenses");                     //firm's rnd expeses, returned as salary to researchers		
			if(v[4]!=0)
				v[1]=v[1]+v[3]*(v[2]/v[4])+v[5];               		//sums up all firms' wage, determined by a unitary wage (sectorial wage divided by firm's productivity) multiplied by firm's effective production plus RND expenses
			else
				v[1]=v[1];
		}
		v[0]=v[0]+v[1];                                          	//sums up all wages of all sectors
	}
	v[6]=V("Government_Effective_Wages");                           //wages paid by the government
	v[7]=v[0]+v[6];                                            		//sums up productive sectors wages with government wages
RESULT(v[7])


EQUATION("Country_Avg_Nominal_Wage")
/*
Stage 5.2: Weighted average nominal wage across all sectors.
Used for unemployment benefits calculation.
Weight = sector employment share.
*/
v[0] = VS(consumption, "Sector_Avg_Wage") * VS(consumption, "Sector_Employment");
v[1] = VS(capital, "Sector_Avg_Wage") * VS(capital, "Sector_Employment");
v[2] = VS(input, "Sector_Avg_Wage") * VS(input, "Sector_Employment");
v[3] = VS(consumption, "Sector_Employment") + VS(capital, "Sector_Employment") + VS(input, "Sector_Employment");
v[4] = (v[3] > 0) ? (v[0] + v[1] + v[2]) / v[3] : VS(consumption, "Sector_Avg_Wage");
RESULT(v[4])


EQUATION("Country_Labor_Force")
/*
Stage 5.2: Total labor force (all worker households).
Computed ONCE per period at country level for efficiency.
Households reference this via VS(country, "Country_Labor_Force") - O(1) lookup.
*/
v[0] = 0;
CYCLE(cur1, "CLASSES")
{
CYCLES(cur1, cur, "HOUSEHOLD")
{
    if(VS(cur, "household_type") == 0)  // Worker
        v[0]++;
}
}
RESULT(max(1, v[0]))


EQUATION("Country_Total_Employment")
/*
Stage 5.2: Total employed workers (in any sector).
Computed ONCE per period at country level for efficiency.
*/
v[0] = 0;
CYCLE(cur1, "CLASSES")
{
CYCLES(cur1, cur, "HOUSEHOLD")
{
    if(VS(cur, "household_type") == 0)  // Worker only
    {
        v[1] = VS(cur, "Household_Employment_Status");
        if(v[1] > 0)  // Employed (status 1, 2, or 3)
            v[0]++;
    }
}
}
RESULT(v[0])


EQUATION("Country_Unemployment_Rate")
/*
Stage 5.2: Unemployment rate = (Labor Force - Employed) / Labor Force
Uses pre-computed country-level aggregates for efficiency.
*/
v[0] = V("Country_Labor_Force");       // Computed once per period
v[1] = V("Country_Total_Employment");  // Computed once per period
v[2] = v[0] - v[1];  // Unemployed
v[3] = (v[0] > 0) ? (v[2] / v[0]) : 0;
RESULT(v[3])


EQUATION("Country_Total_Employed_Skill")
/*
Stage 5.2b: Sum of skill for all employed workers.
Used by Household_Wage_Income for normalized wage distribution.
Ensures identity: SUM(Household_Wage_Income) = Country_Total_Wages
*/
v[0] = 0;
CYCLE(cur1, "CLASSES")
{
CYCLES(cur1, cur, "HOUSEHOLD")
{
    v[1] = VS(cur, "household_type");
    v[2] = VS(cur, "Household_Employment_Status");
    if(v[1] == 0 && v[2] > 0)  // Worker AND employed
    {
        v[3] = VS(cur, "household_skill");
        v[0] += v[3];
    }
}
}
RESULT(max(0.001, v[0]))  // Prevent division by zero


EQUATION("Country_Median_Household_Income")
/*
Stage 4.3 / 7.5: TRUE median household income.

Uses PERCLS(0.5) for actual median calculation.
Used for:
- Income percentile calculations (Household_Income_Percentile)
- Wealth transfer eligibility (Stage 7.5: bottom 50% threshold)

Uses lagged values to avoid circular dependency.
*/
v[0] = household_percentile(country, "Household_Avg_Real_Income", 0.5, 1);  // Custom function for cross-class percentile
RESULT(max(0.01, v[0]))


EQUATION("Country_Total_Investment_Expenses")
/*
Aggeregate Investment Expenses is calculated summing up the demand of capital goods of all firms and multiplying by the average price of the capital goods sector
*/
	v[0]=0;
	CYCLE(cur, "SECTORS")
		v[0]=v[0]+SUMS(cur, "Firm_Effective_Investment_Expenses");
RESULT(v[0])


EQUATION("Country_Profit_Share")
/*
Share of profits over the sum of profits and wages
*/
	v[0]=V("Country_Total_Wages");
	v[1]=V("Country_Total_Profits");
	v[2]=v[0]+v[1];
	v[3]= v[2]!=0? v[1]/v[2] : 0;
RESULT(v[3])


EQUATION("Country_Industrial_Profit_Share")
/*
Share of industrial profits over the sum of profits and wages
*/
	v[0]=0;                                                    		//initializes the CYCLE
	CYCLE(cur, "SECTORS")                                      		//CYCLE trought all sectors
		v[0]=v[0]+SUMS(cur, "Firm_Net_Profits");                    //sums up the surplus of all sectors
	v[1]=V("Country_Total_Wages");
	v[2]=V("Country_Total_Profits");
	v[3]=v[1]+v[2];
	v[4]= v[3]!=0? v[0]/v[3] : 0;
RESULT(v[4])


EQUATION("Country_Financial_Profit_Share")
/*
Share of financial profits over the sum of profits and wages
*/
	v[0]=V("Financial_Sector_Profits");
	v[1]=V("Country_Total_Wages");
	v[2]=V("Country_Total_Profits");
	v[3]=v[1]+v[2];
	v[4]= v[3]!=0? v[0]/v[3] : 0;
RESULT(v[4])


EQUATION("Country_Wage_Share")
/*
Share of profits over the sum of profits and wages
*/
	v[0]=V("Country_Total_Wages");
	v[1]=V("Country_Total_Profits");
	v[2]=v[0]+v[1];
	v[3]= v[2]!=0? v[0]/v[2] : 0;
RESULT(v[3])


EQUATION("Country_Avg_Markup")
/*
Agregated average markup, wheighted by the sales of each sector
*/
	v[0]=WHTAVE("Sector_Avg_Markup", "Sector_Sales");
	v[1]=SUM("Sector_Sales");
	v[2]= v[1]!=0? v[0]/v[1]: 0;
RESULT(v[2])


EQUATION("Country_Debt_Rate_Firms")
/*
Aggregated average debt rate, wheighted by the sales of each sector
*/
	v[0]=WHTAVE("Sector_Avg_Debt_Rate", "Sector_Sales");
	v[1]=SUM("Sector_Sales");
	v[2]= v[1]!=0? v[0]/v[1]: 0;
RESULT(v[2])	


EQUATION("Country_Avg_HHI")
/*
Aggregated average markup, wheighted by the number of firms
*/
	v[0]=WHTAVE("Sector_Normalized_HHI", "Sector_Number_Firms");
	v[1]=SUM("Sector_Number_Firms");
	v[2]= v[1]!=0? v[0]/v[1]: 0;
RESULT(v[2])	


EQUATION("Country_Hedge_Share")
	v[0]=0;
	CYCLE(cur, "SECTORS")
		v[0]=v[0]+SUMS(cur, "firm_hedge");
	v[2]=COUNT_ALL("FIRMS");
	v[3]= v[2]!=0? v[0]/v[2] : 0;
RESULT(v[3])	


EQUATION("Country_Speculative_Share")
	v[0]=0;
	CYCLE(cur, "SECTORS")
		v[0]=v[0]+SUMS(cur, "firm_speculative");
	v[2]=COUNT_ALL("FIRMS");
	v[3]= v[2]!=0? v[0]/v[2] : 0;
RESULT(v[3])


EQUATION("Country_Ponzi_Share")
	v[0]=0;
	CYCLE(cur, "SECTORS")
		v[0]=v[0]+SUMS(cur, "firm_ponzi");
	v[2]=COUNT_ALL("FIRMS");
	v[3]= v[2]!=0? v[0]/v[2] : 0;
RESULT(v[3])


EQUATION("Country_GDP")
/*
Nominal quarterly GDP is calculated summing up profits, wages and indirect taxes
*/
	v[0]=V("Country_Total_Profits");
	v[1]=V("Country_Total_Wages");
	v[2]=V("Government_Indirect_Taxes");
	v[3]=v[0]+v[1]+v[2];
	v[4]=V("Country_GDP_Demand");
	// Diagnostics moved to Diagnostic_Trigger equation
RESULT(v[3])


EQUATION("Diagnostic_Trigger")
/*
Runs after Country_GDP to trigger diagnostic reports.
This avoids circular dependencies by ensuring all macro variables are computed first.
*/
	v[0] = V("Country_GDP");  // Ensure GDP is computed before diagnostics

	// Periodic diagnostic report
	log_diagnostic_report(p, t);

RESULT(1)


EQUATION("Country_Annual_GDP")
RESULT(LAG_SUM(p, "Country_GDP", V("annual_frequency")))


EQUATION("Country_Annual_Real_GDP")
RESULT(LAG_SUM(p, "Country_Real_GDP", V("annual_frequency")))


EQUATION("Country_Real_GDP")
/*
Real quarterly GDP is the nominal GDP over the price index.
*/
	v[0]=V("Country_GDP");              //nominal GDP
	v[1]=V("Country_Price_Index");      //current price index
	v[2]= v[1]!=0? v[0]/v[1] : 0;       //real GDP is the nominal GDP devided by the price index
RESULT(v[2])


EQUATION("Country_Annual_Growth")
/*
Annual Nominal GDP growth rate.
*/
	v[1]=LAG_SUM(p, "Country_GDP", V("annual_frequency"));
	v[2]=LAG_SUM(p, "Country_GDP", V("annual_frequency"), V("annual_frequency") );
	v[3]= v[2]!=0? (v[1]-v[2])/v[2] : 0;
RESULT(v[3])


EQUATION("Country_Annual_Real_Growth")
/*
Annual Real GDP Growth rate.
*/
	v[1]=LAG_SUM(p, "Country_Real_GDP", V("annual_frequency"));
	v[2]=LAG_SUM(p, "Country_Real_GDP", V("annual_frequency"), V("annual_frequency") );
	v[3]= v[2]!=0? (v[1]-v[2])/v[2] : 0;
RESULT(v[3])


EQUATION("Country_Likelihood_Crisis")
/*
Counts the number of crisis ocurrances. 
*/
	v[7]=V("annual_frequency");
	v[0]= fmod((double) t,v[7]);        		//divides the time period by four
	if(v[0]==0)                        		 	//if the rest of the above division is zero (begenning of the year)
		{
		v[1]=V("Country_Annual_Real_Growth");   //real growth rate
		if(v[1]<0)                     			//if the real growth rate is lower the the crisis threshold
			v[3]=1;                         	//counts a crisis
		else                              		//if the real growth rate is not lower the the crisis threshold
			v[3]=0;                         	//do not count a crisis
		}
	else                                		//if the rest of the division is not zero
		v[3]=0;                           		//do not count a crisis   
	v[4]=CURRENT;     							//crisis counter in the last period
	v[5]=v[4]+v[3];                     		//acumulates the crisis counters
	v[6]=(v[5]/t/v[7]);                      	//gives the probability, total crisis counter divided by the number of time periods
RESULT(v[3])


EQUATION("Country_Nominal_Consumption_Production")
RESULT(VS(consumption, "Sector_Sales")*VS(consumption, "Sector_Avg_Price"))

EQUATION("Country_Nominal_Capital_Production")
RESULT(VS(capital, "Sector_Sales")*VS(capital, "Sector_Avg_Price"))

EQUATION("Country_Nominal_Input_Production")
RESULT(VS(input, "Sector_Sales")*VS(input, "Sector_Avg_Price"))

EQUATION("Country_Total_Nominal_Production")
RESULT(WHTAVE("Sector_Avg_Price","Sector_Sales"))

EQUATION("Country_Capacity_Utilization")
/*
Sum up sector's effective production over productive capacity, weighted by sector's nominal value of production over total gross value of production
*/
	v[0]=WHTAVE("Sector_Capacity_Utilization", "Sector_Effective_Production");
	v[1]=SUM("Sector_Effective_Production");
	v[2]= v[1]!=0? v[0]/v[1]: 0;
RESULT(min(1,v[2]))

EQUATION("Country_Idle_Capacity")
RESULT(1-V("Country_Capacity_Utilization"))

EQUATION("Country_Inventories")
RESULT(WHTAVE("Sector_Avg_Price","Sector_Inventories"))


EQUATION("Country_Inventories_Variation")
/*
Sum up the value of changes in iventories of all firms
*/
	v[0]=0;
	CYCLE(cur, "SECTORS")
		v[0]=v[0]+SUMS(cur, "Firm_Inventories_Variation");
RESULT(v[0])


EQUATION("Country_Avg_Productivity")
/*
Average Productivity of the economy weighted by the employment of each sector
*/
	v[0]=WHTAVE("Sector_Avg_Productivity", "Sector_Employment");
	v[1]=SUM("Sector_Employment");
	v[2]= v[1]!=0? v[0]/v[1] : 0;
RESULT(v[2])


EQUATION("Country_Unemployed_Households")
/*
Stage 5.2: Count of unemployed worker households.
Uses lagged employment state to avoid circular dependencies with income calculations.
Capitalist households (state = -1) are excluded from unemployment count.
*/
v[0] = 0;
CYCLE(cur1, "CLASSES")
{
CYCLES(cur1, cur, "HOUSEHOLD")
{
    v[1] = VLS(cur, "Household_Employment_Status", 1);
    if(v[1] == 0)  // Unemployed worker (state = 0)
        v[0] = v[0] + 1;
}
}
RESULT(v[0])


EQUATION("Country_Employed_Households")
/*
Stage 7: Count of employed worker households.
Uses lagged employment state to avoid circular dependencies.
Needed for calculating actual average wage per worker (for unemployment benefits).
*/
v[0] = 0;
CYCLE(cur1, "CLASSES")
{
CYCLES(cur1, cur, "HOUSEHOLD")
{
    v[1] = VLS(cur, "Household_Employment_Status", 1);
    if(v[1] == 1)  // Employed worker (state = 1)
        v[0] = v[0] + 1;
}
}
RESULT(v[0])


EQUATION("Country_GDP_Demand")
/*
GDP calculated by the demand perspective.
Stage 5.5: Added inventory variation for SFC consistency.
Y = C + I + G + X - M + ΔInventories
*/
	v[0]=V("Country_Total_Household_Expenses");       // C
	v[1]=V("Country_Total_Investment_Expenses");      // I
	v[2]=V("Government_Effective_Expenses");          // G
	v[3]=V("Country_Nominal_Exports");                // X
	v[4]=V("Country_Nominal_Imports");                // M
	v[5]=V("Country_Inventories_Variation");          // ΔInventories (SFC fix)
	v[6]=v[0]+v[1]+v[2]+v[3]-v[4]+v[5];              // Y = C+I+G+X-M+ΔInv
RESULT(v[6])


EQUATION("Country_Real_GDP_Demand")
/*
Real quarterly GDP is the nominal GDP over the price index.
*/
	v[0]=V("Country_GDP_Demand");       //nominal GDP
	v[1]=V("Country_Price_Index");      //current price index
	v[2]= v[1]!=0? v[0]/v[1] : 0;   	//real GDP is the nominal GDP devided by the price index
RESULT(v[2])


EQUATION("Country_Total_Household_Expenses")
/*
Stage 4.7 SWITCH #2: Household expenses.
SWITCHED: From SUM("Class_Effective_Expenses")
          To SUMS(households, "Household_Effective_Expenses")
*/
	v[0] = SUMS(working_class, "Household_Effective_Expenses") + SUMS(capitalist_class, "Household_Effective_Expenses");
RESULT(v[0])


/******************************************************************************
 * STAGE 5.3: COUNTRY-LEVEL HOUSEHOLD LOAN AGGREGATES
 * Used by bank equations for cleaner SFC accounting.
 *****************************************************************************/

EQUATION("Country_Total_Household_Stock_Loans")
/*
Stage 5.3: Total household loan stock across all households.
Used by Bank_Stock_Loans_Short_Term for bank accounting.
*/
RESULT(SUMS(working_class, "Household_Stock_Loans") + SUMS(capitalist_class, "Household_Stock_Loans"))


EQUATION("Country_Total_Household_Demand_Loans")
/*
Stage 5.3: Total household loan demand across all households.
Used by Bank_Demand_Loans for credit allocation.
*/
RESULT(SUMS(working_class, "Household_Demand_Loans") + SUMS(capitalist_class, "Household_Demand_Loans"))


EQUATION("Country_Total_Household_Interest_Payment")
/*
Stage 5.3: Total interest payments from all households.
Used by Bank_Interest_Receivment for bank income.
*/
RESULT(SUMS(working_class, "Household_Interest_Payment") + SUMS(capitalist_class, "Household_Interest_Payment"))


EQUATION("Country_Total_Household_Debt_Payment")
/*
Stage 5.3: Total debt amortization from all households.
Used by Bank_Debt_Payment for loan stock reduction.
*/
RESULT(SUMS(working_class, "Household_Debt_Payment") + SUMS(capitalist_class, "Household_Debt_Payment"))


/******************************************************************************
 * STAGE 5.4: FINANCIALIZATION ANALYSIS VARIABLES
 * Track wealth concentration and the decoupling of financial wealth from
 * productive capital (secular stagnation baseline).
 *****************************************************************************/

EQUATION("Country_Total_Financial_Assets")
/*
Stage 5.4: Aggregate financial asset holdings across all households.
Only capitalists hold financial assets (workers have deposits only).
*/
RESULT(SUMS(working_class, "Household_Financial_Assets") + SUMS(capitalist_class, "Household_Financial_Assets"))


EQUATION("Country_Total_Household_Stock_Deposits")
/*
Stage 5.4: Aggregate deposit holdings across all households.
Both workers and capitalists hold deposits.
*/
RESULT(SUMS(working_class, "Household_Stock_Deposits") + SUMS(capitalist_class, "Household_Stock_Deposits"))


EQUATION("Country_Total_Household_Deposits_Return")
/*
Aggregate interest earned on household deposits.
Used by banking sector for interest payments.
*/
RESULT(SUMS(working_class, "Household_Deposits_Return") + SUMS(capitalist_class, "Household_Deposits_Return"))


EQUATION("Country_Income_Tax")
/*
Aggregate income taxation collected from all households.
Used by government for income tax collection.
*/
RESULT(SUMS(working_class, "Household_Income_Taxation") + SUMS(capitalist_class, "Household_Income_Taxation"))


EQUATION("Country_Wealth_Tax_Revenue")
/*
Stage 7: Aggregate wealth tax collected from all households.
Only active when switch_class_tax_structure >= 5.
*/
v[0] = V("switch_class_tax_structure");
if(v[0] < 5)
    v[1] = 0;
else
    v[1] = SUMS(working_class, "Household_Wealth_Tax_Payment") + SUMS(capitalist_class, "Household_Wealth_Tax_Payment");
RESULT(v[1])


EQUATION("Country_Wealth_Tax_Taxpayer_Count")
/*
Stage 7: Count of households paying wealth tax.
Useful for distributional analysis.
*/
v[0] = V("switch_class_tax_structure");
if(v[0] < 5)
{
    v[1] = 0;
}
else
{
    v[1] = 0;
    CYCLE(cur1, "CLASSES")
    {
    CYCLES(cur1, cur, "HOUSEHOLD")
    {
        v[2] = VS(cur, "Household_Wealth_Tax_Payment");
        if(v[2] > 0.01)
            v[1]++;
    }
    }
}
RESULT(v[1])


EQUATION("Country_Wealth_Tax_From_Deposits")
/*
Stage 7: Aggregate wealth tax paid from deposits.
*/
v[0] = V("switch_class_tax_structure");
if(v[0] < 5)
    v[1] = 0;
else
    v[1] = SUMS(working_class, "Household_Wealth_Tax_From_Deposits") + SUMS(capitalist_class, "Household_Wealth_Tax_From_Deposits");
RESULT(v[1])


EQUATION("Country_Wealth_Tax_From_Assets")
/*
Stage 7: Aggregate wealth tax paid via asset liquidation.
*/
v[0] = V("switch_class_tax_structure");
if(v[0] < 5)
    v[1] = 0;
else
    v[1] = SUMS(working_class, "Household_Wealth_Tax_From_Assets") + SUMS(capitalist_class, "Household_Wealth_Tax_From_Assets");
RESULT(v[1])


EQUATION("Country_Wealth_Tax_From_Borrowing")
/*
Stage 7: Aggregate wealth tax paid via borrowing.
*/
v[0] = V("switch_class_tax_structure");
if(v[0] < 5)
    v[1] = 0;
else
    v[1] = SUMS(working_class, "Household_Wealth_Tax_From_Borrowing") + SUMS(capitalist_class, "Household_Wealth_Tax_From_Borrowing");
RESULT(v[1])


EQUATION("Country_Wealth_Capital_Ratio")
/*
Stage 5.4: THE KEY METRIC - v = Total Wealth / Productive Capital.
v > 1 indicates financialization (paper wealth exceeds real capital).

Watch this grow as secular stagnation progresses!
- v ≈ 1.0: Balanced economy (wealth backed by real capital)
- v > 1.5: Financialization (paper wealth decoupled from production)
- v > 2.0: Significant asset bubble territory
*/
v[0] = SUMS(working_class, "Household_Financial_Assets") + SUMS(capitalist_class, "Household_Financial_Assets");  // Paper wealth
v[1] = SUMS(working_class, "Household_Stock_Deposits") + SUMS(capitalist_class, "Household_Stock_Deposits");    // Liquid wealth
v[2] = V("Country_Capital_Stock");
v[3] = (v[2] > 0) ? ((v[0] + v[1]) / v[2]) : 1.0;
RESULT(v[3])


EQUATION("Country_Wealth_GDP_Ratio")
/*
Stage 5.4: Piketty's β = Net Wealth / GDP.
How many years of GDP equals total household wealth.
Typical values: β = 4-6 in developed economies.
Uses lagged GDP to avoid circular dependency with diagnostics.
*/
v[0] = V("Country_Total_Household_Net_Wealth");
v[1] = VL("Country_GDP", 1);  // Lagged to avoid deadlock (diagnostic called from Country_GDP)
v[2] = (v[1] > 0) ? (v[0] / v[1]) : 0;
RESULT(v[2])


EQUATION("Country_Capitalist_Wealth_Share")
/*
Stage 5.4: Fraction of total household wealth held by capitalists.
Should approach 1.0 as financialization progresses (wealth concentration).

Total Wealth = Financial Assets + Deposits - Loans (for each household)
*/
v[0] = 0;  // Capitalist wealth
v[1] = 0;  // Worker wealth
CYCLE(cur1, "CLASSES")
{
CYCLES(cur1, cur, "HOUSEHOLD")
{
    v[2] = VS(cur, "household_type");
    v[3] = VS(cur, "Household_Financial_Assets");
    v[4] = VS(cur, "Household_Stock_Deposits");
    v[5] = VS(cur, "Household_Stock_Loans");
    v[6] = v[3] + v[4] - v[5];  // Net wealth

    if(v[2] == 1)
        v[0] += v[6];  // Capitalist
    else
        v[1] += v[6];  // Worker
}
}
v[7] = v[0] + v[1];
v[8] = (v[7] > 0) ? (v[0] / v[7]) : 0;
RESULT(v[8])


EQUATION("Country_Total_Household_Net_Wealth")
/*
Stage 5.4: Total net wealth across all households.
Net Wealth = Deposits + Financial Assets - Loans
*/
RESULT(SUMS(working_class, "Household_Net_Wealth") + SUMS(capitalist_class, "Household_Net_Wealth"))


/******************************************************************************
 * STAGE 9: TAX EVASION & CAPITAL FLIGHT AGGREGATIONS
 *****************************************************************************/


EQUATION("Country_Total_Deposits_Offshore")
/*
Stage 9: Total deposits held in offshore tax havens.
Invisible to government, not subject to wealth tax.
*/
RESULT(SUMS(working_class, "Household_Deposits_Offshore") + SUMS(capitalist_class, "Household_Deposits_Offshore"))


EQUATION("Country_Total_Assets_Undeclared")
/*
Stage 9: Total financial assets not declared to tax authority.
Subject to audit risk and penalties if caught.
*/
RESULT(SUMS(working_class, "Household_Assets_Undeclared") + SUMS(capitalist_class, "Household_Assets_Undeclared"))


EQUATION("Country_Total_Assets_Declared")
/*
Stage 9: Total financial assets declared to tax authority.
*/
RESULT(SUMS(working_class, "Household_Assets_Declared") + SUMS(capitalist_class, "Household_Assets_Declared"))


EQUATION("Country_Capital_Flight_Rate")
/*
Stage 9: Fraction of total deposits held offshore.
= Offshore_Deposits / Total_Deposits
*/
v[0] = V("Country_Total_Deposits_Offshore");
v[1] = V("Country_Total_Household_Stock_Deposits");
v[2] = (v[1] > 0.01) ? v[0] / v[1] : 0;
RESULT(v[2])


EQUATION("Country_Asset_Evasion_Rate")
/*
Stage 9: Fraction of financial assets undeclared.
= Undeclared_Assets / Total_Financial_Assets
*/
v[0] = V("Country_Total_Assets_Undeclared");
v[1] = SUMS(working_class, "Household_Financial_Assets") + SUMS(capitalist_class, "Household_Financial_Assets");
v[2] = (v[1] > 0.01) ? v[0] / v[1] : 0;
RESULT(v[2])


EQUATION("Country_Evader_Count")
/*
Stage 9: Number of households with hidden wealth.
= Offshore_Deposits > 0 OR Undeclared_Assets > 0
*/
v[0] = 0;
CYCLE(cur1, "CLASSES")
{
CYCLES(cur1, cur, "HOUSEHOLD")
{
    v[1] = VS(cur, "Household_Deposits_Offshore");
    v[2] = VS(cur, "Household_Assets_Undeclared");
    if(v[1] > 0.01 || v[2] > 0.01)
        v[0] += 1;
}
}
RESULT(v[0])


EQUATION("Country_Audit_Count")
/*
Stage 9: Number of households audited this period.
*/
RESULT(SUMS(working_class, "Household_Is_Audited") + SUMS(capitalist_class, "Household_Is_Audited"))


EQUATION("Country_Penalty_Revenue")
/*
Stage 9: Total penalties collected from caught evaders.
Goes to Government_Penalty_Revenue.
*/
RESULT(SUMS(working_class, "Household_Asset_Penalty") + SUMS(capitalist_class, "Household_Asset_Penalty"))


// Country_Tax_Gap moved to Government_Wealth_Tax_Gap in fun_government.h (Stage 9)


EQUATION("Country_Productive_Capacity_Depreciated")
/*
Sum up the value of depreciated productive capacity of all firms
*/
	v[0]=0;
	CYCLE(cur, "SECTORS")
		v[0]=v[0]+SUMS(cur, "Firm_Productive_Capacity_Depreciation");
RESULT(v[0])


EQUATION("Country_Productive_Capacity_Expansion")
/*
Sum up the value of productive capacity for expanstion of all firms
*/
	v[0]=0;
	CYCLE(cur, "SECTORS")
		v[0]=v[0]+SUMS(cur, "Firm_Demand_Productive_Capacity_Expansion");
RESULT(v[0])


EQUATION("Country_Productive_Capacity_Replacement")
/*
Sum up the value of productive capacity for replacement of all firms
*/
	v[0]=0;
	CYCLE(cur, "SECTORS")
		v[0]=v[0]+SUMS(cur, "Firm_Demand_Productive_Capacity_Replacement");
RESULT(v[0])


EQUATION("Country_Capital_Stock")
/*
Sum up the nominal value of firms stock of capital
*/
	v[0]=0;
	CYCLE(cur, "SECTORS")
		v[0]=v[0]+SUMS(cur, "Firm_Capital");
RESULT(v[0])


EQUATION("Country_Capital_Output_Ratio")
/*
Observed Ratio, Stock of Capital over GDP
*/
	v[0]=V("Country_GDP");
	v[1]=V("Country_Capital_Stock");
	v[2]= v[0]!=0? v[1]/v[0] : 0;
RESULT(v[2])


EQUATION("Country_Capital_Labor_Ratio")
/*
Observed Ratio, Stock of Capital over Total Employment
*/
	v[0]=SUM("Sector_Employment");
	v[1]=V("Country_Capital_Stock");
	v[2]= v[0]!=0? v[1]/v[0] : 0;
RESULT(v[2])


EQUATION("Country_Avg_Profit_Rate")
/*
Observed Ratio, Total Profits over Stock of Capital
*/
	v[0]=V("Country_Total_Profits")-V("Financial_Sector_Profits");
	v[1]=V("Country_Capital_Stock");
	v[2]= v[1]!=0? v[0]/v[1] : 0;
RESULT(v[2])


EQUATION("Country_Induced_Investment")
/*
Sum up the nominal value of effective expansion investment of all firms
*/
	v[0]=0;
	CYCLE(cur, "SECTORS")
		v[0]=v[0]+SUMS(cur, "Firm_Effective_Expansion_Investment_Expenses");
RESULT(v[0])


EQUATION("Country_Autonomous_Investment")
/*
Sum up the nominal value of effective replacement investment of all firms
*/
	v[0]=0;
	CYCLE(cur, "SECTORS")
		v[0]=v[0]+SUMS(cur, "Firm_Replacement_Expenses");
RESULT(v[0])


EQUATION("Country_Autonomous_Consumption")
/*
Stage 4.7 SWITCH #3: Household autonomous consumption.
Sum up nominal value of autonomous consumption.
SWITCHED: From SUM("Class_Real_Autonomous_Consumption")
          To SUMS(households, "Household_Real_Autonomous_Consumption")
*/
	v[0]=SUMS(working_class, "Household_Real_Autonomous_Consumption") + SUMS(capitalist_class, "Household_Real_Autonomous_Consumption");
	v[1]=VS(consumption, "Sector_Avg_Price");
	v[2]=v[0]*v[1];
RESULT(v[2])


EQUATION("Country_Debt_Rate")
/*
Aggregated average household debt rate, weighted by income.
Debt Rate = Stock_Loans / Stock_Deposits for each household.
*/
	v[10] = 0;  // weighted sum of debt rates
	v[11] = 0;  // sum of incomes (weights)
	CYCLE(cur1, "CLASSES")
	{
	CYCLES(cur1, cur, "HOUSEHOLD")
	{
		v[0] = VS(cur, "Household_Stock_Deposits");
		v[1] = VS(cur, "Household_Stock_Loans");
		v[2] = VS(cur, "Household_Nominal_Disposable_Income");
		v[3] = (v[0] > 0.001) ? v[1] / v[0] : 0;  // debt rate = loans/deposits
		v[10] += v[3] * v[2];  // weighted by income
		v[11] += v[2];
	}
	}
	v[12] = (v[11] > 0.001) ? v[10] / v[11] : 0;
RESULT(v[12])


/******************************************************************************
 * OPTIMIZED INEQUALITY INDICES
 *
 * Performance optimizations:
 * 1. std::sort instead of bubble sort: O(n log n) vs O(n²)
 * 2. Master equations compute all indices in single pass
 * 3. Data collected once, sorted once, all shares computed together
 * 4. Individual equations are thin wrappers returning pre-computed values
 ******************************************************************************/

EQUATION("Country_Gini_Index")
/*
MASTER EQUATION for income inequality (disposable, post-tax).
Computes: Gini, Palma, Top10, Top1, Bottom50, Theil
Uses std::sort for O(n log n) performance.
WRITEs results to individual variables.
*/
	v[0] = COUNTS(working_class, "HOUSEHOLD") + COUNTS(capitalist_class, "HOUSEHOLD");
	i = (int) v[0];
	if(i <= 1)
	{
		WRITE("Country_Palma_Ratio_Income", 0);
		WRITE("Country_Top10_Share_Income", 0);
		WRITE("Country_Top1_Share_Income", 0);
		WRITE("Country_Bottom50_Share_Income", 0);
		WRITE("Country_Theil_Index_Income", 0);
		END_EQUATION(0);
	}

	double* values = new double[i];
	double total = 0;
	j = 0;
	CYCLE(cur1, "CLASSES")
	{
	CYCLES(cur1, cur, "HOUSEHOLD")
	{
		values[j] = VS(cur, "Household_Nominal_Disposable_Income");
		total += values[j];
		j++;
	}
	}

	// Compute Theil BEFORE sorting (doesn't need sorted data)
	double mean = total / i;
	double theil = 0;
	for(int k = 0; k < i; k++) {
		double val = max(1e-10, values[k]);
		double ratio = val / mean;
		if(ratio > 1e-10)
			theil += ratio * log(ratio);
	}
	theil /= i;
	WRITE("Country_Theil_Index_Income", theil);

	// Sort using std::sort - O(n log n)
	std::sort(values, values + i);

	// Compute Gini from sorted array
	double sum_ix = 0, sum_x = 0;
	for(int k = 0; k < i; k++) {
		sum_ix += (k + 1) * values[k];
		sum_x += values[k];
	}
	double gini = (sum_x > 1e-10) ? (2.0 * sum_ix - (i + 1) * sum_x) / (i * sum_x) : 0;

	// Compute all shares in single pass through sorted array
	int bottom40_idx = (int)(i * 0.4);
	int bottom50_idx = (int)(i * 0.5);
	int top10_idx = (int)(i * 0.9);
	int top1_idx = (int)(i * 0.99);
	if(top1_idx >= i) top1_idx = i - 1;

	double bottom40_sum = 0, bottom50_sum = 0, top10_sum = 0, top1_sum = 0;
	for(int k = 0; k < i; k++) {
		if(k < bottom40_idx) bottom40_sum += values[k];
		if(k < bottom50_idx) bottom50_sum += values[k];
		if(k >= top10_idx) top10_sum += values[k];
		if(k >= top1_idx) top1_sum += values[k];
	}

	// Write all computed values
	WRITE("Country_Palma_Ratio_Income", (fabs(bottom40_sum) > 1e-10) ? top10_sum / bottom40_sum : 9999);
	WRITE("Country_Top10_Share_Income", (total > 1e-10) ? top10_sum / total : 0);
	WRITE("Country_Top1_Share_Income", (total > 1e-10) ? top1_sum / total : 0);
	WRITE("Country_Bottom50_Share_Income", (total > 1e-10) ? bottom50_sum / total : 0);

	delete[] values;
RESULT(gini)

EQUATION("Country_Gini_Index_Pretax")
/*
Gini coefficient for gross (pre-tax) income distribution.
Standalone equation (different data source than post-tax).
*/
	v[0] = COUNTS(working_class, "HOUSEHOLD") + COUNTS(capitalist_class, "HOUSEHOLD");
	i = (int) v[0];
	if(i <= 1)
		END_EQUATION(0);

	double* values = new double[i];
	j = 0;
	CYCLE(cur1, "CLASSES")
	{
	CYCLES(cur1, cur, "HOUSEHOLD")
	{
		values[j] = VS(cur, "Household_Nominal_Gross_Income");
		j++;
	}
	}

	// Sort and compute Gini
	std::sort(values, values + i);
	double sum_ix = 0, sum_x = 0;
	for(int k = 0; k < i; k++) {
		sum_ix += (k + 1) * values[k];
		sum_x += values[k];
	}
	double gini = (sum_x > 1e-10) ? (2.0 * sum_ix - (i + 1) * sum_x) / (i * sum_x) : 0;

	delete[] values;
RESULT(gini)

EQUATION("Country_Gini_Index_Wealth")
/*
MASTER EQUATION for wealth inequality (post-tax).
Computes: Gini, Palma, Top10, Top1, Bottom50, Theil
Uses std::sort for O(n log n) performance.
*/
	v[0] = COUNTS(working_class, "HOUSEHOLD") + COUNTS(capitalist_class, "HOUSEHOLD");
	i = (int) v[0];
	if(i <= 1)
	{
		WRITE("Country_Palma_Ratio_Wealth", 0);
		WRITE("Country_Top10_Share_Wealth", 0);
		WRITE("Country_Top1_Share_Wealth", 0);
		WRITE("Country_Bottom50_Share_Wealth", 0);
		WRITE("Country_Theil_Index_Wealth", 0);
		END_EQUATION(0);
	}

	double* values = new double[i];
	double total = 0;
	j = 0;
	CYCLE(cur1, "CLASSES")
	{
	CYCLES(cur1, cur, "HOUSEHOLD")
	{
		values[j] = VS(cur, "Household_Net_Wealth");
		total += values[j];
		j++;
	}
	}

	// Compute Theil BEFORE sorting
	double mean = total / i;
	double theil = 0;
	for(int k = 0; k < i; k++) {
		double val = max(1e-10, values[k]);
		double ratio = val / mean;
		if(ratio > 1e-10)
			theil += ratio * log(ratio);
	}
	theil /= i;
	WRITE("Country_Theil_Index_Wealth", theil);

	// Sort using std::sort
	std::sort(values, values + i);

	// Compute Gini
	double sum_ix = 0, sum_x = 0;
	for(int k = 0; k < i; k++) {
		sum_ix += (k + 1) * values[k];
		sum_x += values[k];
	}
	double gini = (sum_x > 1e-10) ? (2.0 * sum_ix - (i + 1) * sum_x) / (i * sum_x) : 0;

	// Compute all shares
	int bottom40_idx = (int)(i * 0.4);
	int bottom50_idx = (int)(i * 0.5);
	int top10_idx = (int)(i * 0.9);
	int top1_idx = (int)(i * 0.99);
	if(top1_idx >= i) top1_idx = i - 1;

	double bottom40_sum = 0, bottom50_sum = 0, top10_sum = 0, top1_sum = 0;
	for(int k = 0; k < i; k++) {
		if(k < bottom40_idx) bottom40_sum += values[k];
		if(k < bottom50_idx) bottom50_sum += values[k];
		if(k >= top10_idx) top10_sum += values[k];
		if(k >= top1_idx) top1_sum += values[k];
	}

	WRITE("Country_Palma_Ratio_Wealth", (fabs(bottom40_sum) > 1e-10) ? top10_sum / bottom40_sum : 9999);
	WRITE("Country_Top10_Share_Wealth", (total > 1e-10) ? top10_sum / total : 0);
	WRITE("Country_Top1_Share_Wealth", (total > 1e-10) ? top1_sum / total : 0);
	WRITE("Country_Bottom50_Share_Wealth", (total > 1e-10) ? bottom50_sum / total : 0);

	delete[] values;
RESULT(gini)

EQUATION("Country_Gini_Index_Wealth_Pretax")
/*
MASTER EQUATION for wealth inequality (pre-tax).
Computes: Gini, Palma, Top10, Top1, Bottom50, Theil
Pre-tax wealth = current net wealth + wealth tax paid this period.
*/
	v[0] = COUNTS(working_class, "HOUSEHOLD") + COUNTS(capitalist_class, "HOUSEHOLD");
	i = (int) v[0];
	if(i <= 1)
	{
		WRITE("Country_Palma_Ratio_Wealth_Pretax", 0);
		WRITE("Country_Top10_Share_Wealth_Pretax", 0);
		WRITE("Country_Top1_Share_Wealth_Pretax", 0);
		WRITE("Country_Bottom50_Share_Wealth_Pretax", 0);
		WRITE("Country_Theil_Index_Wealth_Pretax", 0);
		END_EQUATION(0);
	}

	double* values = new double[i];
	double total = 0;
	j = 0;
	CYCLE(cur1, "CLASSES")
	{
	CYCLES(cur1, cur, "HOUSEHOLD")
	{
		values[j] = VS(cur, "Household_Net_Wealth") + VS(cur, "Household_Wealth_Tax_Payment");
		total += values[j];
		j++;
	}
	}

	// Compute Theil BEFORE sorting
	double mean = total / i;
	double theil = 0;
	for(int k = 0; k < i; k++) {
		double val = max(1e-10, values[k]);
		double ratio = val / mean;
		if(ratio > 1e-10)
			theil += ratio * log(ratio);
	}
	theil /= i;
	WRITE("Country_Theil_Index_Wealth_Pretax", theil);

	// Sort using std::sort
	std::sort(values, values + i);

	// Compute Gini
	double sum_ix = 0, sum_x = 0;
	for(int k = 0; k < i; k++) {
		sum_ix += (k + 1) * values[k];
		sum_x += values[k];
	}
	double gini = (sum_x > 1e-10) ? (2.0 * sum_ix - (i + 1) * sum_x) / (i * sum_x) : 0;

	// Compute all shares
	int bottom40_idx = (int)(i * 0.4);
	int bottom50_idx = (int)(i * 0.5);
	int top10_idx = (int)(i * 0.9);
	int top1_idx = (int)(i * 0.99);
	if(top1_idx >= i) top1_idx = i - 1;

	double bottom40_sum = 0, bottom50_sum = 0, top10_sum = 0, top1_sum = 0;
	for(int k = 0; k < i; k++) {
		if(k < bottom40_idx) bottom40_sum += values[k];
		if(k < bottom50_idx) bottom50_sum += values[k];
		if(k >= top10_idx) top10_sum += values[k];
		if(k >= top1_idx) top1_sum += values[k];
	}

	WRITE("Country_Palma_Ratio_Wealth_Pretax", (fabs(bottom40_sum) > 1e-10) ? top10_sum / bottom40_sum : 9999);
	WRITE("Country_Top10_Share_Wealth_Pretax", (total > 1e-10) ? top10_sum / total : 0);
	WRITE("Country_Top1_Share_Wealth_Pretax", (total > 1e-10) ? top1_sum / total : 0);
	WRITE("Country_Bottom50_Share_Wealth_Pretax", (total > 1e-10) ? bottom50_sum / total : 0);

	delete[] values;
RESULT(gini)


/******************************************************************************
 * INEQUALITY INDICES: Dummy Variables (Income)
 * These are computed by Country_Gini_Index via WRITE()
 ******************************************************************************/

EQUATION_DUMMY("Country_Palma_Ratio_Income", "Country_Gini_Index")
EQUATION_DUMMY("Country_Top10_Share_Income", "Country_Gini_Index")
EQUATION_DUMMY("Country_Top1_Share_Income", "Country_Gini_Index")
EQUATION_DUMMY("Country_Bottom50_Share_Income", "Country_Gini_Index")
EQUATION_DUMMY("Country_Theil_Index_Income", "Country_Gini_Index")


/******************************************************************************
 * INEQUALITY INDICES: Dummy Variables (Wealth, post-tax)
 * These are computed by Country_Gini_Index_Wealth via WRITE()
 ******************************************************************************/

EQUATION_DUMMY("Country_Palma_Ratio_Wealth", "Country_Gini_Index_Wealth")
EQUATION_DUMMY("Country_Top10_Share_Wealth", "Country_Gini_Index_Wealth")
EQUATION_DUMMY("Country_Top1_Share_Wealth", "Country_Gini_Index_Wealth")
EQUATION_DUMMY("Country_Bottom50_Share_Wealth", "Country_Gini_Index_Wealth")
EQUATION_DUMMY("Country_Theil_Index_Wealth", "Country_Gini_Index_Wealth")


/******************************************************************************
 * INEQUALITY INDICES: Dummy Variables (Wealth, pre-tax)
 * These are computed by Country_Gini_Index_Wealth_Pretax via WRITE()
 ******************************************************************************/

EQUATION_DUMMY("Country_Palma_Ratio_Wealth_Pretax", "Country_Gini_Index_Wealth_Pretax")
EQUATION_DUMMY("Country_Top10_Share_Wealth_Pretax", "Country_Gini_Index_Wealth_Pretax")
EQUATION_DUMMY("Country_Top1_Share_Wealth_Pretax", "Country_Gini_Index_Wealth_Pretax")
EQUATION_DUMMY("Country_Bottom50_Share_Wealth_Pretax", "Country_Gini_Index_Wealth_Pretax")
EQUATION_DUMMY("Country_Theil_Index_Wealth_Pretax", "Country_Gini_Index_Wealth_Pretax")


EQUATION("Country_Avg_Propensity_Consume")
/*
Stage 4.7 SWITCH #4: Household-based average propensity.
SWITCHED: From SUM("Class_*") To SUMS(households, "Household_*")
*/
	v[0]=VS(consumption, "Sector_Avg_Price");
	v[1]=SUMS(working_class, "Household_Effective_Real_Domestic_Consumption") + SUMS(capitalist_class, "Household_Effective_Real_Domestic_Consumption");
	v[2]=SUMS(working_class, "Household_Nominal_Disposable_Income") + SUMS(capitalist_class, "Household_Nominal_Disposable_Income");
	v[3]= v[2]!=0? v[0]*v[1]/v[2] : 0;
RESULT(v[3])


/******************************************************************************
 * STAGE 7.5: WEALTH TRANSFER (Equal Distribution to Bottom X%)
 *
 * OPTIMIZED POLICY:
 * - Eligibility: Bottom X% by income (parameter: wealth_transfer_target_percentile)
 * - Distribution: Equal transfer to all eligible households
 * - Budget: Wealth tax revenue (goes through government budget allocation)
 * - OPTIMIZATION: Eligibility flag written during count (no double check)
 *
 * Flow: Wealth_Tax → Country_Transfer_Desired → Gov_Budget → Equal split
 *
 * REQUIRED:
 * - wealth_transfer_target_percentile (COUNTRY level parameter, default 0.5)
 * - Household_Transfer_Eligible (HOUSEHOLD level EQUATION_DUMMY, written here)
 *****************************************************************************/


EQUATION("Country_Transfer_Desired")
/*
Stage 7.5 OPTIMIZED: Equal transfer to bottom X% of households.

BUDGET SOURCE: Wealth tax revenue (goes through government budget allocation).
ELIGIBILITY: Income <= threshold (PERCLS with wealth_transfer_target_percentile).
OPTIMIZATION: Marks each household's eligibility via WRITES during the count loop,
              so Household_Transfer_Received doesn't need to recompute.

Parameter: wealth_transfer_target_percentile (default 0.5 = median = bottom 50%)

Flow: Wealth_Tax → Desired → Gov_Budget → Effective → Equal split to households
*/
v[0] = VS(country, "switch_class_tax_structure");
if(v[0] < 5)
{
    // Clear all eligibility flags
    CYCLE(cur1, "CLASSES")
    {
    CYCLES(cur1, cur, "HOUSEHOLD")
    {
        WRITES(cur, "Household_Transfer_Eligible", 0);
    }
    }
    WRITE("Country_Transfer_Eligible", 0);
    v[10] = 0;
}
else
{
    // Get target percentile parameter (default 0.5 = median)
    v[1] = V("wealth_transfer_target_percentile");
    if(v[1] <= 0 || v[1] > 1) v[1] = 0.5;  // Safety default

    // Calculate income threshold at target percentile (cross-class)
    v[2] = household_percentile(country, "Household_Avg_Real_Income", v[1], 1);

    // Count eligible AND mark each household in ONE pass
    v[3] = 0;  // Eligible count
    CYCLE(cur1, "CLASSES")
    {
    CYCLES(cur1, cur, "HOUSEHOLD")
    {
        v[4] = VLS(cur, "Household_Avg_Real_Income", 1);
        if(v[4] <= v[2])  // Income at or below threshold
        {
            WRITES(cur, "Household_Transfer_Eligible", 1);
            v[3]++;
        }
        else
        {
            WRITES(cur, "Household_Transfer_Eligible", 0);
        }
    }
    }

    WRITE("Country_Transfer_Eligible", v[3]);

    // BUDGET = Wealth tax revenue
    v[10] = VS(country, "Country_Wealth_Tax_Revenue");
}
RESULT(max(0, v[10]))


EQUATION("Country_Transfer_Eligible")
/*
Stage 7.5: Count of households eligible for wealth transfer (bottom X%).
Value is set via WRITE in Country_Transfer_Desired.
*/
RESULT(CURRENT)

