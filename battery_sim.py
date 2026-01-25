import numpy as np
from scipy.optimize import linprog
import matplotlib.pyplot as plt
import time

class BatterySimulator:
    def __init__(self, capacity_mwh=100.0, max_charge_mw=50 * 50/57, max_discharge_mw=50.0, buy_discount=-7/57, max_daily_charge_mwh=200.0):
        self.capacity = capacity_mwh
        self.max_charge = max_charge_mw
        self.max_discharge = max_discharge_mw
        self.buy_discount = buy_discount
        self.max_daily_charge = max_daily_charge_mwh

    def solve(self, prices, initial_soc=0.0):
        """
        Solves for the optimal charge/discharge strategy to maximize profit.
        prices: list or array of prices for each hour.
        initial_soc: starting energy in the battery (MWh).
        """
        n_hours = len(prices)
        n_days = n_hours // 24
        
        # Decision Variables:
        # x[0:n]     -> Charge power c_t (MW)
        # x[n:2*n]   -> Discharge power d_t (MW)
        # x[2*n:3*n] -> State of Charge e_t at end of hour t (MWh)
        
        # Objective: Maximize sum(price_t * d_t - (1 - discount) * price_t * c_t)
        # linprog minimizes, so negate the objective.
        # min sum((1 - discount) * price_t * c_t - price_t * d_t)
        
        c_buy = (1 - self.buy_discount) * np.array(prices)
        c_sell = np.array(prices)
        
        obj = np.zeros(3 * n_hours)
        obj[0:n_hours] = c_buy          # Cost of buying
        obj[n_hours:2*n_hours] = -c_sell # Revenue from selling (negated for minimization)
        obj[2*n_hours:3*n_hours] = 0    # Energy levels don't directly enter objective
        
        # Constraints:
        # 1. Energy Balance: e_t = e_{t-1} + c_t - d_t  =>  e_t - e_{t-1} - c_t + d_t = 0
        A_eq = []
        b_eq = []
        
        for t in range(n_hours):
            row = np.zeros(3 * n_hours)
            row[t] = -1             # -c_t
            row[n_hours + t] = 1    # +d_t
            row[2 * n_hours + t] = 1 # +e_t
            if t > 0:
                row[2 * n_hours + (t - 1)] = -1 # -e_{t-1}
            A_eq.append(row)
            # For t=0, e_{-1} = initial_soc => e_0 - c_0 + d_0 = initial_soc
            b_eq.append(initial_soc if t == 0 else 0)
            
        # 2. Daily Throughput constraints: Sum(c_t over day d) <= max_daily_charge
        A_ub = []
        b_ub = []
        
        for d in range(n_days):
            row_throughput = np.zeros(3 * n_hours)
            start_h = d * 24
            end_h = min((d + 1) * 24, n_hours)
            row_throughput[start_h:end_h] = 1 # sum of c_t for this day
            A_ub.append(row_throughput)
            b_ub.append(self.max_daily_charge)
            
        # Bounds:
        # 0 <= c_t <= max_charge
        # 0 <= d_t <= max_discharge
        # 0 <= e_t <= capacity
        bounds = []
        for _ in range(n_hours): bounds.append((0, self.max_charge))      # c_t
        for _ in range(n_hours): bounds.append((0, self.max_discharge))   # d_t
        for _ in range(n_hours): bounds.append((0, self.capacity))        # e_t
        
        # Solve
        start_time = time.time()
        res = linprog(obj, A_eq=np.array(A_eq), b_eq=np.array(b_eq), 
                      A_ub=np.array(A_ub), b_ub=np.array(b_ub),
                      bounds=bounds, method='highs')
        end_time = time.time()
        print(f"Solver took {end_time - start_time:.4f} seconds for {n_hours} hours")
        
        if res.success:
            c = res.x[0:n_hours]
            d = res.x[n_hours:2*n_hours]
            e = res.x[2*n_hours:3*n_hours]
            profit = -res.fun
            return {
                'charge': c,
                'discharge': d,
                'soc': e,
                'profit': profit,
                'prices': prices
            }
        else:
            raise ValueError(f"Optimization failed: {res.message}")

def plot_results(results, filename='simulation_results.png', days_to_plot=7):
    n_hours = len(results['prices'])
    all_hours = np.arange(n_hours)
    
    # For a full year, we want to show a summary but also a zoomed-in view of the first week
    plot_hours = days_to_plot * 24
    hours = all_hours[:plot_hours]
    prices = results['prices'][:plot_hours]
    charge = results['charge'][:plot_hours]
    discharge = results['discharge'][:plot_hours]
    soc = results['soc'][:plot_hours]
    
    fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(12, 12), sharex=True)
    
    # Plot 1: Prices
    ax1.step(hours, prices, where='post', label='Price', color='gold', linewidth=1.5)
    ax1.set_ylabel('Price ($/MWh)')
    ax1.set_title(f'Energy Market Prices (First {days_to_plot} days of {n_hours//24} days)')
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    # Plot 2: Charge/Discharge Power
    ax2.bar(hours, charge, label='Charge', color='green', alpha=0.6, width=1.0)
    ax2.bar(hours, -discharge, label='Discharge', color='red', alpha=0.6, width=1.0)
    ax2.set_ylabel('Power (MW)')
    ax2.set_title('Battery Operation (Charge + / Discharge -)')
    ax2.legend()
    ax2.grid(True, alpha=0.3)
    
    # Plot 3: State of Charge
    ax3.fill_between(hours, soc, color='blue', alpha=0.3, label='SoC')
    ax3.plot(hours, soc, color='blue', linewidth=1)
    ax3.set_ylabel('Energy (MWh)')
    ax3.set_xlabel('Hour of Year')
    ax3.set_title(f'Battery State of Charge (Total Year Profit: ${results["profit"]:,.2f})')
    ax3.legend()
    ax3.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig(filename)
    print(f"Plot saved to {filename}")

if __name__ == "__main__":
    # Set seed for reproducibility
    np.random.seed(42)
    
    # Generate 1 year of data (8760 hours)
    n_hours = 8760
    hours = np.arange(n_hours)
    
    # Seasonality: higher prices in winter/summer, lower in spring/autumn
    seasonal_factor = 20 * np.sin(2 * np.pi * hours / (24 * 365))
    # Daily cycle: peak in morning and evening
    daily_cycle = 30 * np.sin(2 * np.pi * hours / 24 - np.pi/2)
    # Noise
    noise = np.random.normal(0, 50, n_hours)
    
    prices = 50 + seasonal_factor + daily_cycle + noise
    
    sim = BatterySimulator()
    results = sim.solve(prices)
    
    print(f"Optimal Strategy Found for the Year!")
    print(f"Total Annual Profit: ${results['profit']:,.2f}")
    
    plot_results(results)
