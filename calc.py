decile_data = {
    "D1": {"Salarios": 27656, "EOB/RMB": 17607},
    "D2": {"Salarios": 68158, "EOB/RMB": 29020},
    "D3": {"Salarios": 99366, "EOB/RMB": 37052},
    "D4": {"Salarios": 137101, "EOB/RMB": 46247},
    "D5": {"Salarios": 184680, "EOB/RMB": 59677},
    "D6": {"Salarios": 221135, "EOB/RMB": 73379},
    "D7": {"Salarios": 279933, "EOB/RMB": 92797},
    "D8": {"Salarios": 369955, "EOB/RMB": 122760},
    "D9": {"Salarios": 545628, "EOB/RMB": 188407},
    "D10":{"Salarios": 1284826, "EOB/RMB": 514197}
}

# Initial calculations for C4, C5, C6 from raw decile data (can be kept for reference if needed)
# Class 4_orig: Top 10% (D10)
wages_c4_orig = decile_data["D10"]["Salarios"]
profits_c4_orig = decile_data["D10"]["EOB/RMB"]
total_c4_orig = wages_c4_orig + profits_c4_orig
wage_share_c4_orig = wages_c4_orig / total_c4_orig if total_c4_orig else 0
profit_share_c4_orig = profits_c4_orig / total_c4_orig if total_c4_orig else 0

# Class 5_orig: Middle 40% (D6+D7+D8+D9)
wages_c5_orig = (decile_data["D6"]["Salarios"] +
            decile_data["D7"]["Salarios"] +
            decile_data["D8"]["Salarios"] +
            decile_data["D9"]["Salarios"])
profits_c5_orig = (decile_data["D6"]["EOB/RMB"] +
              decile_data["D7"]["EOB/RMB"] +
              decile_data["D8"]["EOB/RMB"] +
              decile_data["D9"]["EOB/RMB"])
total_c5_orig = wages_c5_orig + profits_c5_orig
wage_share_c5_orig = wages_c5_orig / total_c5_orig if total_c5_orig else 0
profit_share_c5_orig = profits_c5_orig / total_c5_orig if total_c5_orig else 0

# Class 6_orig: Bottom 50% (D1+D2+D3+D4+D5)
wages_c6_orig = (decile_data["D1"]["Salarios"] +
            decile_data["D2"]["Salarios"] +
            decile_data["D3"]["Salarios"] +
            decile_data["D4"]["Salarios"] +
            decile_data["D5"]["Salarios"])
profits_c6_orig = (decile_data["D1"]["EOB/RMB"] +
              decile_data["D2"]["EOB/RMB"] +
              decile_data["D3"]["EOB/RMB"] +
              decile_data["D4"]["EOB/RMB"] +
              decile_data["D5"]["EOB/RMB"])
total_c6_orig = wages_c6_orig + profits_c6_orig
wage_share_c6_orig = wages_c6_orig / total_c6_orig if total_c6_orig else 0
profit_share_c6_orig = profits_c6_orig / total_c6_orig if total_c6_orig else 0

# print("Original Wage and Profit Share Proxies based on Figura 4 deciles (for reference):")
# print("--------------------------------------------------------------------------------")
# print(f"Class 4 (Top 10% - D10): Wage Share: {wage_share_c4_orig:.4f}, Profit Share: {profit_share_c4_orig:.4f}")
# print(f"Class 5 (Middle 40% - D6-D9): Wage Share: {wage_share_c5_orig:.4f}, Profit Share: {profit_share_c5_orig:.4f}")
# print(f"Class 6 (Bottom 50% - D1-D5): Wage Share: {wage_share_c6_orig:.4f}, Profit Share: {profit_share_c6_orig:.4f}\n")
# print("Note: Classes 1 (top 0.01%), 2 (next 0.09%), and 3 (next 0.9%) were not directly calculable from raw deciles.\n")


# Grand totals from Figura 4 data (sum of all D1-D10 values)
gt_wages_fig4 = sum(d["Salarios"] for d in decile_data.values())
gt_profits_fig4 = sum(d["EOB/RMB"] for d in decile_data.values())
total_model_income_fig4_check = gt_wages_fig4 + gt_profits_fig4 # Total income based on sum of all deciles

print("\n\n--- APPLYING USER-DEFINED SIMULATION SHARES TO FIGURA 4 TOTALS ---")
print("--------------------------------------------------------------------")
print(f"Using Grand Total Wages (from Figura 4 D1-D10 sum): {gt_wages_fig4:.2f}")
print(f"Using Grand Total Profits (from Figura 4 D1-D10 sum): {gt_profits_fig4:.2f}\n")

# User-defined target shares for the final distribution of total profit and wage bills
target_profit_bill_shares = {
    "Class 1 (Top 0.01%)": 0.25,
    "Class 2 (Next 0.09%)": 0.15,
    "Class 3 (Next 0.9%)": 0.20,
    "Class 4 (Next 9%)": 0.20,
    "Class 5 (Middle 40%)": 0.15,
    "Class 6 (Bottom 50%)": 0.05
}

target_wage_bill_shares = {
    "Class 1 (Top 0.01%)": 0.10,
    "Class 2 (Next 0.09%)": 0.15,
    "Class 3 (Next 0.9%)": 0.25,
    "Class 4 (Next 9%)": 0.25,
    "Class 5 (Middle 40%)": 0.15,
    "Class 6 (Bottom 50%)": 0.10
}

print("Target Profit Bill Distribution:")
for cn, s in target_profit_bill_shares.items(): print(f"  {cn}: {s*100:.2f}%")
print("\nTarget Wage Bill Distribution:")
for cn, s in target_wage_bill_shares.items(): print(f"  {cn}: {s*100:.2f}%")
print("\n")


# Calculate absolute wages, profits, and total income for each class based on these target shares
simulation_derived_data = {}

# Define all class names for iteration, matching keys in target_profit/wage_bill_shares
all_class_names_for_sim = [
    "Class 1 (Top 0.01%)", "Class 2 (Next 0.09%)", "Class 3 (Next 0.9%)",
    "Class 4 (Next 9%)", "Class 5 (Middle 40%)", "Class 6 (Bottom 50%)"
]


for class_name in all_class_names_for_sim:
    abs_profits = target_profit_bill_shares[class_name] * gt_profits_fig4
    abs_wages = target_wage_bill_shares[class_name] * gt_wages_fig4
    total_income = abs_profits + abs_wages
    simulation_derived_data[class_name] = {
        "Wages": abs_wages,
        "Profits": abs_profits,
        "Total Income": total_income,
        "Wage Share (internal)": abs_wages / total_income if total_income else 0,
        "Profit Share (internal)": abs_profits / total_income if total_income else 0
    }

print("Derived Class Data (Wages, Profits, Internal Shares) based on Simulation Targets:")
# Printing in a defined order (e.g. richest to poorest, or user-defined order)
ordered_print_classes = [
    "Class 1 (Top 0.01%)", "Class 2 (Next 0.09%)", "Class 3 (Next 0.9%)",
    "Class 4 (Next 9%)", "Class 5 (Middle 40%)", "Class 6 (Bottom 50%)"
]
for class_name in ordered_print_classes:
    data = simulation_derived_data[class_name]
    print(f"{class_name}:")
    print(f"  Total Income: {data['Total Income']:.2f}")
    print(f"  Absolute Wages: {data['Wages']:.2f}")
    print(f"  Absolute Profits: {data['Profits']:.2f}")
    print(f"  Internal Wage Share: {data['Wage Share (internal)']:.4f}")
    print(f"  Internal Profit Share: {data['Profit Share (internal)']:.4f}\n")


# --- Distribution of Total Wages and Total Profits Across Classes ---
# This section will now confirm the user's target distributions
print("\n\nDistribution of Total Wages and Total Profits Across Classes (based on Simulation Targets):")
print("---------------------------------------------------------------------------------------------")

grand_total_wages_sim_check = sum(simulation_derived_data[cn]["Wages"] for cn in all_class_names_for_sim)
grand_total_profits_sim_check = sum(simulation_derived_data[cn]["Profits"] for cn in all_class_names_for_sim)

print(f"Grand Total of All Wages (Calculated from shares): {grand_total_wages_sim_check:.2f} (Should match Figura 4 D1-D10 sum: {gt_wages_fig4:.2f})")
print(f"Grand Total of All Profits (Calculated from shares): {grand_total_profits_sim_check:.2f} (Should match Figura 4 D1-D10 sum: {gt_profits_fig4:.2f})\n")

for class_name in ordered_print_classes: 
    wage_amount = simulation_derived_data[class_name]["Wages"]
    profit_amount = simulation_derived_data[class_name]["Profits"]

    share_of_total_wages = (wage_amount / gt_wages_fig4) * 100 if gt_wages_fig4 else 0
    share_of_total_profits = (profit_amount / gt_profits_fig4) * 100 if gt_profits_fig4 else 0
    
    print(f"{class_name}:")
    print(f"  Absolute Wages: {wage_amount:.2f} | Share of Total Wages: {share_of_total_wages:.2f}% (Target: {target_wage_bill_shares[class_name]*100:.2f}%)")
    print(f"  Absolute Profits: {profit_amount:.2f} | Share of Total Profits: {share_of_total_profits:.2f}% (Target: {target_profit_bill_shares[class_name]*100:.2f}%)\n")

# Verification sums
sum_dist_wage_shares_sim = sum((simulation_derived_data[cn]["Wages"] / gt_wages_fig4 * 100) for cn in all_class_names_for_sim)
sum_dist_profit_shares_sim = sum((simulation_derived_data[cn]["Profits"] / gt_profits_fig4 * 100) for cn in all_class_names_for_sim)

print(f"Sum of 'Share of Total Wages (%)' across all classes (Simulation): {sum_dist_wage_shares_sim:.2f}% (should be 100%)")
print(f"Sum of 'Share of Total Profits (%)' across all classes (Simulation): {sum_dist_profit_shares_sim:.2f}% (should be 100%)")


# --- GINI COEFFICIENT CALCULATION (based on Simulation-derived incomes) ---
print("\n\nCalculating Gini Coefficient from Simulation-Derived Class Incomes:")
print("--------------------------------------------------------------------")

population_shares_map = { # This remains the same as defined by user
    "Class 1 (Top 0.01%)": 0.0001,
    "Class 2 (Next 0.09%)": 0.0009,
    "Class 3 (Next 0.9%)": 0.009,
    "Class 4 (Next 9%)": 0.09,
    "Class 5 (Middle 40%)": 0.40,
    "Class 6 (Bottom 50%)": 0.50
}

class_incomes_for_gini_sim = {
    cn: simulation_derived_data[cn]["Total Income"] for cn in all_class_names_for_sim
}

# Gini calculation requires classes ordered from poorest to richest
ordered_class_names_for_gini = [
    "Class 6 (Bottom 50%)", "Class 5 (Middle 40%)", "Class 4 (Next 9%)",
    "Class 3 (Next 0.9%)", "Class 2 (Next 0.09%)", "Class 1 (Top 0.01%)"
]

class_data_for_gini_sim = []
for name in ordered_class_names_for_gini: 
    pop_share = population_shares_map[name]
    income_val = class_incomes_for_gini_sim[name]
    class_data_for_gini_sim.append({
        "name": name,
        "pop_share": pop_share,
        "income": income_val
    })

total_economy_income_gini_sim = sum(item['income'] for item in class_data_for_gini_sim)

cum_pop_sim = 0.0
cum_income_share_sim = 0.0
lorenz_points_sim = [(0.0, 0.0)]

for item in class_data_for_gini_sim:
    cum_pop_sim += item['pop_share']
    cum_income_share_sim += (item['income'] / total_economy_income_gini_sim if total_economy_income_gini_sim > 0 else 0)
    lorenz_points_sim.append((cum_pop_sim, cum_income_share_sim))

gini_sum_term_sim = 0.0
for i in range(1, len(lorenz_points_sim)):
    p_i_minus_1, l_i_minus_1 = lorenz_points_sim[i-1]
    p_i, l_i = lorenz_points_sim[i]
    # The group_pop_share should correspond to the interval ending at p_i, l_i
    # class_data_for_gini_sim is ordered poorest to richest, so index i-1 is correct for the i-th segment
    group_pop_share = class_data_for_gini_sim[i-1]['pop_share'] 
    gini_sum_term_sim += group_pop_share * (l_i + l_i_minus_1) # Correct formula uses sum of ordinates for the segment

calculated_gini_sim = 1 - gini_sum_term_sim

print(f"Calculated Gini Index (from Simulation-derived Incomes): {calculated_gini_sim:.4f}")
# The "provided Gini" (e.g. 0.69) is not actively used to calibrate in this script version.
# If you have a specific Gini this simulation should result in, you can compare it manually.
# Example:
# target_simulation_gini = 0.69 
# print(f"Target Gini for this simulation (if any): {target_simulation_gini}")
# print(f"Difference: {calculated_gini_sim - target_simulation_gini:.4f}")

# End of script