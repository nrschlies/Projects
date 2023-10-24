"""
N-Queens Solver: Simulated Annealing & Hill Climbing with Sidestep

This script provides solutions for the N-Queens problem using:
1. Simulated Annealing
2. Hill Climbing with Sidestep
3. Depth First Search

Usage:
    Execute the script to run a 100-trial simulation comparing both techniques.

Author: Noah Schliesman
"""
import random as rd
import time
import math
import sys
import tkinter as tk
from tkinter import messagebox
from tkinter import ttk
import threading
from PIL import Image, ImageTk
import queue

class NQueensGUI:
    def __init__(self):
        self.root = tk.Tk()
        self.queue = queue.Queue()

        self.root.title("N-Queens Solver")
        self.simulation = Simulation(100, self, self.root)

        self.moves_label = tk.Label(self.root)
        self.moves_label.pack(pady=10)

        self.board_frame = tk.Frame(self.root)
        self.board_frame.pack(pady=20)
        
        self.buttons_frame = tk.Frame(self.root)
        self.buttons_frame.pack(pady=20)

        self.board_buttons = []
 
        self.light_square_color = "#DDB88C"
        self.dark_square_color = "#A66D4F"
    
        queen_image = Image.open("/Users/nrschlies/Desktop/imresizer-1697687475771.png")
        queen_image = queen_image.resize((50, 50), Image.Resampling.LANCZOS)
        self.queen_icon = ImageTk.PhotoImage(queen_image)

        for i in range(8):
            row = []
            for j in range(8):
                canvas = tk.Canvas(self.board_frame, width=50, height=50, bg=self.dark_square_color if (i + j) % 2 else self.light_square_color)
                canvas.grid(row=i, column=j)
                row.append(canvas)
            self.board_buttons.append(row)
        
        self.update_board_display()

        tk.Button(self.buttons_frame, text="Solve with SA", command=self.solve_with_annealing).pack(side=tk.LEFT, padx=10)
        tk.Button(self.buttons_frame, text="Solve with HC", command=self.solve_with_hill_climbing).pack(side=tk.LEFT, padx=10)
        tk.Button(self.buttons_frame, text="Solve with DFS", command=self.solve_with_dfs).pack(side=tk.LEFT, padx=10)

    def run_simulations(self):
        self._run_simulations()

    def _run_simulations(self):
        # Run the simulation for all methods and get the results
        results_data = self.simulation.testing_simulation(self.simulation.num_trials)
        formatted_results = "Simulation Results:\n\n"
        for method, data in results_data.items():
            formatted_results += f"{method}:\n"
            formatted_results += f"  Success Percentage: {data['Success Percentage']:.2f}%\n"
            formatted_results += f"  Average moves: {data['Average Moves']}\n"
            formatted_results += f"  Average time: {data['Average Time']:.2f} ms\n"
            formatted_results += "\n"

        # Directly update the results_label here
        self.results_label['text'] = formatted_results

        return formatted_results

    def update_moves_display(self, current_moves, total_moves):
        #if total_moves is None:
            #self.moves_label.config(text=f"Moves: {current_moves}")
            self.moves_label.config(text=f"(Total Moves to Solution: {total_moves})")

    def update_board_display(self):
        for i in range(8):
            for j in range(8):
                btn = self.board_buttons[i][j]
                if (i + j) % 2 == 0:
                    btn.config(bg=self.light_square_color)
                else:
                    btn.config(bg=self.dark_square_color)
                if (i, j) in self.simulation.board.positions:
                    canvas_item = self.board_buttons[i][j]
                    canvas_item.create_image(29, 25, image=self.queen_icon)
                else:
                    canvas_item = self.board_buttons[i][j]
                    canvas_item.delete("all")
                    canvas_item.config(bg=self.light_square_color if (i + j) % 2 == 0 else self.dark_square_color)

    def solve_with_annealing(self, delay=0.01):
        # Use a thread to keep the GUI responsive
        t = threading.Thread(target=self._solve_with_annealing, args=(delay,))
        t.start()

    def _solve_with_annealing(self, delay):
        _, moves = self.simulation.solver.simulated_annealing(delay)
            
        self.animate_solution() # Start the animation
        messagebox.showinfo("Result", f"Solved with Simulated Annealing in {moves} moves!")

    def solve_with_hill_climbing(self):
        # Use a thread to keep the GUI responsive
        t = threading.Thread(target=self._solve_with_hill_climbing())
        t.start()

    def _solve_with_hill_climbing(self):
        _, moves = self.simulation.solver.hill_climbing_with_sidestep()

        self.animate_solution() 

        self.animate_solution()
        messagebox.showinfo("Result", f"Solved with Hill Climbing in {moves} moves!")

    def solve_with_dfs(self, delay=0.01):
        # Use a thread to keep the GUI responsive
        t = threading.Thread(target=self._solve_with_dfs)
        t.start()

    def _solve_with_dfs(self):
            self.simulation.board.initialize()
            _, moves = self.simulation.solver.depth_first_search()

            print(f"Number of states stored for animation: {len(self.simulation.solver.history)}")  # Debug line
        
            self.animate_solution()
            messagebox.showinfo("Result", f"Solved with Depth First Search in {moves} moves!")
    
    def run(self):
        self.poll_queue()
        self.root.mainloop()

    def poll_queue(self):
        try:
            msg = self.queue.get(0)  # Try to get a message from the queue
            self.results_label['text'] = msg  # Update the results label
        except queue.Empty:
            pass
        self.root.after(100, self.poll_queue)

    def animate_solution(self, delay=0.05):
        if not self.simulation.solver.history:
            return
        board_state = self.simulation.solver.history.pop(0)
        self.simulation.board.positions = board_state

        total_moves = len(self.simulation.solver.history) + 1  # 1 is added for the current move.
        current_move = total_moves - len(self.simulation.solver.history)
        
        self.update_moves_display(current_move, total_moves)
        
        self.update_board_display()
        if self.simulation.solver.history:
            self.root.after(int(delay*1000), self.animate_solution, delay)  # Convert delay to ms



class Board:
    def __init__(self, n):
        self.n = n
        self.positions = self.initialize()

    def initialize(self):
        positions = [(i, j) for i in range(self.n) for j in range(self.n)]
        return rd.sample(positions, self.n)

    def display(self):
        for row in range(self.n):
            row_string = ['.'] * self.n
            for queen in self.positions:
                if queen[0] == row:
                    row_string[queen[1]] = 'Q'
            print(' '.join(row_string))
        print()

    def cost(self):
        collisions = 0
        for i in range(self.n):
            for j in range(i+1, self.n):
                if (self.positions[i][0] == self.positions[j][0] or 
                    self.positions[i][1] == self.positions[j][1] or 
                    abs(self.positions[i][0] - self.positions[j][0]) == abs(self.positions[i][1] - self.positions[j][1])):
                    collisions += 1
        return collisions / 2


    def print_progress_bar(self, iteration, total, prefix='', length=50, fill='█'):
        progress = (iteration / total) * 100
        self.gui.update_progress_bar(progress)
        if self.gui:
            self.print_progress_bar(i+1, num_trials)
        else:
            percent = ("{0:.1f}").format(100 * (iteration / float(total)))
            filled_length = int(length * iteration // total)
            bar = fill * filled_length + '-' * (length - filled_length)
            sys.stdout.write(f'\r{prefix} |{bar}| {percent}% Complete')
            sys.stdout.flush()

    def move_queen_randomly(self):
        idx = rd.randint(0, self.n-1)
        self.positions[idx] = (idx, rd.randint(0, self.n-1))

    def get_most_troublesome_queen(self):
        conflicts = [0] * self.n
        for i in range(self.n):
            for j in range(self.n):
                if (i != j and 
                    (self.positions[i][0] == self.positions[j][0] or 
                    self.positions[i][1] == self.positions[j][1] or 
                    abs(self.positions[i][0] - self.positions[j][0]) == abs(self.positions[i][1] - self.positions[j][1]))):
                    conflicts[i] += 1
        return conflicts.index(max(conflicts))

    def move_queen_efficiently(self):
        idx = self.get_most_troublesome_queen()
        original_col = self.positions[idx][1]
        min_conflicts = self.n
        best_col = original_col
        for col in range(self.n):
            self.positions[idx] = (idx, col)
            current_conflicts = self.cost()
            if current_conflicts < min_conflicts:
                best_col = col
                min_conflicts = current_conflicts
        self.positions[idx] = (idx, best_col)

class Solver: 
    def __init__(self, board, gui=None):
        self.board = board
        self.gui = gui
        self.found_solution = False
        self.history = []

    def simulated_annealing(self, delay=0.05):
        self.history.clear()
        n = 8
        self.board.positions = self.board.initialize()
        move_counter = 0
        current_cost = self.board.cost()
        best_cost = current_cost
        temperature = 900.0
        alpha = .999
        moves_without_improvement = 0
        max_moves_without_improvement = 1

        while temperature > 100:
            prev_board = self.board.positions[:]
            if rd.random() < 0.95:
                self.board.move_queen_efficiently()
                
            else:
                self.board.move_queen_randomly()

            if not self.history or self.history[-1] != self.board.positions:
                    self.history.append([tuple(pos) for pos in self.board.positions]) 

            if self.gui:
                self.gui.update_board_display()
                time.sleep(delay)

            move_counter += 1
            new_cost = self.board.cost()
            delta_cost = (current_cost - new_cost) / (temperature)# + 1e-9)
            ap = math.exp(delta_cost) if abs(delta_cost) < 900 else 0
            if ap > rd.random() or new_cost < current_cost:
                current_cost = new_cost
                if new_cost < best_cost:
                    best_cost = new_cost
                    moves_without_improvement = 0
                else:
                    moves_without_improvement += 1
            else:
                self.board.positions = prev_board
                moves_without_improvement += 1

            if moves_without_improvement >= max_moves_without_improvement:
                idx_to_change = rd.randint(0, n-1)
                new_position = (idx_to_change, rd.randint(0, n-1))
                self.board.positions[idx_to_change] = new_position
                current_cost = self.board.cost()
                moves_without_improvement = 0
            if current_cost == 0:
                return self.board.positions, move_counter
            temperature *= alpha
        
        return None, move_counter

    def best_two_moves(self):
        n = len(self.board.positions)
        min_cost = self.board.cost()
        best_i = -1
        best_j = -1
        for i in range(n):
            current_pos = self.board.positions[i]
            for j in range(n):
                if j != current_pos[1]:
                    self.board.positions[i] = (i, j)
                    current_cost = self.board.cost()
                    if current_cost < min_cost:
                        min_cost = current_cost
                        best_i = i
                        best_j = j
                    self.board.positions[i] = current_pos

        return best_i, best_j

    def is_safe(self, row, col):
        for i in range(row):
            if (self.board.positions[i][1] == col or
                self.board.positions[i][1] - self.board.positions[i][0] == col - row or
                self.board.positions[i][0] + self.board.positions[i][1] == row + col):
                return False
        return True

    def dfs(self, row):
        n = len(self.board.positions)

        if row == n:
            self.history.append([tuple(pos) for pos in self.board.positions])
            self.found_solution = True  # Mark that a solution has been found
            return

        for col in range(n):
            if self.is_safe(row, col):
                self.board.positions[row] = (row, col)  # Place queen
                self.dfs(row + 1)  # Explore further without immediately returning



    def depth_first_search(self):
        # Reset board positions    
        self.board.positions = self.board.initialize()
        self.history.clear()  # Clear the history before the search

        self.found_solution = False  # Reset this before starting DFS
        self.dfs(0)

        if self.found_solution:
            return [(i, self.board.positions[i]) for i in range(len(self.board.positions))], len(self.history)
        else:
            return None, len(self.history)
        
    def hill_climbing_with_sidestep(self, delay=0.05):
        self.board.positions = self.board.initialize()
        max_moves_without_improvement = 1
        total_moves_limit = 2000
        restarts = 0
        moves_without_improvement = 0
        moves = 0
        past_boards = set()

        while moves < total_moves_limit:
            current_cost = self.board.cost()
            
            if current_cost == 0:
                return self.board.positions, moves
            
            past_boards.add(tuple(sorted(self.board.positions)))  # Store the current board configuration
            
            best_i, best_j = self.best_two_moves()

            # Prioritize the best move. If not available, consider sidestep.
            if best_i != -1:
                self.board.positions[best_i] = (best_i, best_j)
                if not self.history or self.history[-1] != self.board.positions:
                    self.history.append([tuple(pos) for pos in self.board.positions])
                if self.gui:
                    self.gui.update_board_display()
                    time.sleep(delay)
                moves += 1
                moves_without_improvement = 0
            else:
                # Try to move the most troublesome queen first
                most_troublesome_queen = self.board.get_most_troublesome_queen()
                self.board.positions[most_troublesome_queen] = (most_troublesome_queen, rd.randint(0, len(self.board.positions)-1))
                if not self.history or self.history[-1] != self.board.positions:
                    self.history.append([tuple(pos) for pos in self.board.positions])
                if self.gui:
                    self.gui.update_board_display()
                    time.sleep(delay)
                moves += 1
                
                # If the new configuration is something we've seen recently, do a random sidestep
                while tuple(sorted(self.board.positions)) in past_boards:
                    self.board.move_queen_randomly()
                    if not self.history or self.history[-1] != self.board.positions:
                        self.history.append([tuple(pos) for pos in self.board.positions])
                    if self.gui:
                        self.gui.update_board_display()
                        time.sleep(delay)
                    moves += 1
                
                moves_without_improvement += 1

            if moves_without_improvement >= max_moves_without_improvement:
                # Instead of a complete restart, adjust positions of a few queens
                for _ in range(rd.randint(1, 4)):   
                    self.board.move_queen_randomly()
                    if not self.history or self.history[-1] != self.board.positions:
                        self.history.append([tuple(pos) for pos in self.board.positions])
                    if self.gui:
                        self.gui.update_board_display()
                        time.sleep(delay)
                    moves += 1
                moves_without_improvement = 0
                restarts += 1

        if self.board.cost() == 0:
            return self.board.positions, moves
        return None, moves

        
class Simulation:
    def __init__(self, num_trials, gui=None, root=None):
        self.num_trials = num_trials
        self.gui = gui
        self.root = root  # Add this line
        self.board = Board(8)  # Assuming 8 queens, but this can be changed.
        self.solver = Solver(self.board, gui)

    def run(self):
        self.testing_simulation(self.num_trials)
        self.solve_and_display("depth_first")  # or whatever method you want as the default
    
    def print_progress_bar(self, iteration, total, prefix='', length=50, fill='█'):
        progress = (iteration / total) * 100
        self.gui.update_progress_bar(progress)  
        """
        percent = ("{0:.1f}").format(100 * (iteration / float(total)))
        filled_length = int(length * iteration // total)
        bar = fill * filled_length + '-' * (length - filled_length)
        sys.stdout.write(f'\r{prefix} |{bar}| {percent}% Complete')
        sys.stdout.flush()
        """

    def testing_simulation(self, num_trials):
        sa_solvable_count = 0
        hc_solvable_count = 0
        dfs_solvable_count = 0
        sa_total_moves = 0
        hc_total_moves = 0
        dfs_total_moves = 0
        sa_total_time = 0 
        hc_total_time = 0
        dfs_total_time = 0  

        print("Running simulations:")
        for i in range(num_trials):
            #self.gui.update_progress_bar((i + 1) * 100 / num_trials)
            start_time = time.time()  
            dfs_solution, dfs_moves = self.solver.depth_first_search()
            dfs_total_time += time.time() - start_time  
            if dfs_solution:
                dfs_solvable_count += 1
                dfs_total_moves += dfs_moves

            start_time = time.time()  
            sa_solution, sa_moves = self.solver.simulated_annealing()
            sa_total_time += time.time() - start_time  
            if sa_solution:
                sa_solvable_count += 1
                sa_total_moves += sa_moves

            start_time = time.time()  
            hc_solution, hc_moves = self.solver.hill_climbing_with_sidestep()
            hc_total_time += time.time() - start_time  
            if hc_solution:
                hc_solvable_count += 1
                hc_total_moves += hc_moves

        print()  

        sa_empirical_percentage = 100 * sa_solvable_count / num_trials
        hc_empirical_percentage = 100 * hc_solvable_count / num_trials
        dfs_empirical_percentage = 100 * dfs_solvable_count / num_trials

        sa_avg_moves = sa_total_moves / sa_solvable_count if sa_solvable_count > 0 else 0
        hc_avg_moves = hc_total_moves / hc_solvable_count if hc_solvable_count > 0 else 0
        dfs_avg_moves = dfs_total_moves / dfs_solvable_count if dfs_solvable_count > 0 else 0

        sa_avg_time_ms = 1000 * sa_total_time / sa_solvable_count if sa_solvable_count > 0 else 0  
        hc_avg_time_ms = 1000 * hc_total_time / hc_solvable_count if hc_solvable_count > 0 else 0  
        dfs_avg_time_ms = 1000 * dfs_total_time / dfs_solvable_count if dfs_solvable_count > 0 else 0  

        print(f"[Depth First Search] Success Percentage: {dfs_empirical_percentage:.2f}%")
        print(f"[Depth First Search] Average moves: {dfs_avg_moves}")
        print(f"[Depth First Search] Average time: {dfs_avg_time_ms:.2f} ms")

        print(f"[Hill Climbing with Sidestep] Success Percentage: {hc_empirical_percentage:.2f}%")
        print(f"[Hill Climbing with Sidestep] Average moves: {hc_avg_moves}")
        print(f"[Hill Climbing with Sidestep] Average time: {hc_avg_time_ms:.2f} ms")

        print(f"[Simulated Annealing] Success Percentage: {sa_empirical_percentage:.2f}%")
        print(f"[Simulated Annealing] Average moves: {sa_avg_moves}")
        print(f"[Simulated Annealing] Average time: {sa_avg_time_ms:.2f} ms")

        results = {
            "DFS": {
                "Success Percentage": dfs_empirical_percentage,
                "Average Moves": dfs_avg_moves,
                "Average Time": dfs_avg_time_ms
            },
            "HC with Sidestep": {
                "Success Percentage": hc_empirical_percentage,
                "Average Moves": hc_avg_moves,
                "Average Time": hc_avg_time_ms
            },
            "Simulated Annealing": {
                "Success Percentage": sa_empirical_percentage,
                "Average Moves": sa_avg_moves,
                "Average Time": sa_avg_time_ms
            }
        }

        return results
    
    def solve_and_display(self, method):
        """
        Initializes a board, displays its initial state, solves it using the specified method, 
        and then displays the solved state and number of moves.
        
        Parameters:
            method (str): The method to use for solving, either "annealing" or "hill_climbing".
        """
        self.board.initialize()

        # Print initial board
        print("Initial Board:")
        self.board.display()

        # Solve the board based on the specified method
        if method == "annealing":
            solved_board, moves = self.solver.simulated_annealing()
        elif method == "hill_climbing":
            solved_board, moves = self.solver.hill_climbing_with_sidestep()
        elif method == "depth_first":
            solved_board, moves = self.solver.depth_first_search()
        else:
            print(f"Invalid method: {method}")
            return

        if solved_board:
            print("\nSolved Board after", moves, "moves:")
            self.board.display()
        else:
            print("\nNo solution found after", moves, "moves.")

if __name__ == "__main__":
    gui = NQueensGUI()
    gui.run()
