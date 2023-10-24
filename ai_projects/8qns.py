"""
N-Queens Solver: Simulated Annealing & Hill Climbing with Sidestep

This script provides solutions for the N-Queens problem using:
1. Simulated Annealing
2. Hill Climbing with Sidestep

Usage:
    Execute the script to run a 100-trial simulation comparing both techniques.

Author: Noah Schliesman
"""
import random as rd
import time
import math
import sys

def initialize_board(n):
    positions = [(i, j) for i in range(n) for j in range(n)]
    return rd.sample(positions, n)

def print_board(board):
    n = len(board)
    for row in range(n):
        row_string = ['.'] * n
        for queen in board:
            if queen[0] == row:
                row_string[queen[1]] = 'Q'
        print(' '.join(row_string))
    print()

def cost(board):
    n = len(board)
    collisions = 0
    for i in range(n):
        for j in range(i+1, n):
            if (board[i][0] == board[j][0] or 
                board[i][1] == board[j][1] or 
                abs(board[i][0] - board[j][0]) == abs(board[i][1] - board[j][1])):
                collisions += 1
    return collisions / 2

def print_progress_bar(iteration, total, prefix='', length=50, fill='█'):
    percent = ("{0:.1f}").format(100 * (iteration / float(total)))
    filled_length = int(length * iteration // total)
    bar = fill * filled_length + '-' * (length - filled_length)
    sys.stdout.write(f'\r{prefix} |{bar}| {percent}% Complete')
    sys.stdout.flush()

def move_queen_randomly(board):
    idx = rd.randint(0, len(board)-1)
    board[idx] = (idx, rd.randint(0, len(board)-1))

def get_most_troublesome_queen(board):
    conflicts = [0] * len(board)
    for i in range(len(board)):
        for j in range(len(board)):
            if (i != j and 
                (board[i][0] == board[j][0] or 
                board[i][1] == board[j][1] or 
                abs(board[i][0] - board[j][0]) == abs(board[i][1] - board[j][1]))):
                conflicts[i] += 1
    return conflicts.index(max(conflicts))

def move_queen_efficiently(board):
    idx = get_most_troublesome_queen(board)
    original_col = board[idx][1]
    min_conflicts = len(board)
    best_col = original_col
    for col in range(len(board)):
        board[idx] = (idx, col)
        current_conflicts = cost(board)
        if current_conflicts < min_conflicts:
            best_col = col
            min_conflicts = current_conflicts
    board[idx] = (idx, best_col)

def simulated_annealing():
    n = 8
    board = initialize_board(n)
    move_counter = 0
    current_cost = cost(board)
    best_cost = current_cost
    temperature = 100.0
    alpha = 0.999
    moves_without_improvement = 0
    max_moves_without_improvement = 4

    while temperature > 10:
        prev_board = board[:]
        if rd.random() < 0.7:
            move_queen_efficiently(board)
        else:
            move_queen_randomly(board)
        move_counter += 1
        new_cost = cost(board)
        delta_cost = (current_cost - new_cost) / (temperature + 1e-9)
        ap = math.exp(delta_cost) if abs(delta_cost) < 500 else 0
        if ap > rd.random() or new_cost < current_cost:
            current_cost = new_cost
            if new_cost < best_cost:
                best_cost = new_cost
                moves_without_improvement = 0
            else:
                moves_without_improvement += 1
        else:
            board = prev_board
            moves_without_improvement += 1

        if moves_without_improvement >= max_moves_without_improvement:
            idx_to_change = rd.randint(0, n-1)
            new_position = (idx_to_change, rd.randint(0, n-1))
            board[idx_to_change] = new_position
            current_cost = cost(board)
            moves_without_improvement = 0
        if current_cost == 0:
            return board, move_counter
        temperature *= alpha
    return None, move_counter

def best_two_moves(board):
    n = len(board)
    min_cost = cost(board)
    best_i = -1
    best_j = -1
    for i in range(n):
        current_pos = board[i]
        for j in range(n):
            if j != current_pos[1]:
                board[i] = (i, j)
                current_cost = cost(board)
                if current_cost < min_cost:
                    min_cost = current_cost
                    best_i = i
                    best_j = j
                board[i]
                board[i] = current_pos

    return best_i, best_j

def hill_climbing_with_sidestep():
    board = initialize_board(8)
    max_moves_without_improvement = 1
    total_moves_limit = 2000
    restarts = 0
    moves_without_improvement = 0
    moves = 0
    past_boards = set()

    while moves < total_moves_limit:
        current_cost = cost(board)
        
        if current_cost == 0:
            return board, moves
        
        past_boards.add(tuple(sorted(board)))  # Store the current board configuration
        
        best_i, best_j = best_two_moves(board)

        # Prioritize the best move. If not available, consider sidestep.
        if best_i != -1:
            board[best_i] = (best_i, best_j)
            moves_without_improvement = 0
        else:
            # Try to move the most troublesome queen first
            most_troublesome_queen = get_most_troublesome_queen(board)
            board[most_troublesome_queen] = (most_troublesome_queen, rd.randint(0, len(board)-1))
            
            # If the new configuration is something we've seen recently, do a random sidestep
            while tuple(sorted(board)) in past_boards:
                move_queen_randomly(board)
            
            moves_without_improvement += 1

        moves += 1

        if moves_without_improvement >= max_moves_without_improvement:
            # Instead of a complete restart, adjust positions of a few queens
            for _ in range(rd.randint(1, 3)):  
                move_queen_randomly(board)
            moves_without_improvement = 0
            restarts += 1

    if cost(board) == 0:
        return board, moves
    return None, moves



def testing_simulation(num_trials):
    sa_solvable_count = 0
    hc_solvable_count = 0
    sa_total_moves = 0
    hc_total_moves = 0
    sa_total_time = 0 
    hc_total_time = 0  

    print("Running simulations:")
    for i in range(num_trials):
        print_progress_bar(i + 1, num_trials, prefix='Progress', length=50)

        start_time = time.time()  
        sa_solution, sa_moves = simulated_annealing()
        sa_total_time += time.time() - start_time  

        start_time = time.time()  
        hc_solution, hc_moves = hill_climbing_with_sidestep()
        hc_total_time += time.time() - start_time  

        if sa_solution:
            sa_solvable_count += 1
            sa_total_moves += sa_moves
        if hc_solution:
            hc_solvable_count += 1
            hc_total_moves += hc_moves
    print()  

    sa_empirical_percentage = 100 * sa_solvable_count / num_trials
    hc_empirical_percentage = 100 * hc_solvable_count / num_trials

    sa_avg_moves = sa_total_moves / sa_solvable_count if sa_solvable_count > 0 else 0
    hc_avg_moves = hc_total_moves / hc_solvable_count if hc_solvable_count > 0 else 0

    sa_avg_time_ms = 1000 * sa_total_time / sa_solvable_count if sa_solvable_count > 0 else 0  
    hc_avg_time_ms = 1000 * hc_total_time / hc_solvable_count if hc_solvable_count > 0 else 0  

    print(f"[Simulated Annealing] Success Percentage: {sa_empirical_percentage:.2f}%")
    print(f"[Simulated Annealing] Average moves: {sa_avg_moves}")
    print(f"[Simulated Annealing] Average time: {sa_avg_time_ms:.2f} ms")
    print(f"[Hill Climbing with Sidestep] Success Percentage: {hc_empirical_percentage:.2f}%")
    print(f"[Hill Climbing with Sidestep] Average moves: {hc_avg_moves}")
    print(f"[Hill Climbing with Sidestep] Average time: {hc_avg_time_ms:.2f} ms")

def solve_and_display(method):
    """
    Initializes a board, displays its initial state, solves it using the specified method, 
    and then displays the solved state and number of moves.
    
    Parameters:
        method (str): The method to use for solving, either "annealing" or "hill_climbing".
    """
    board = initialize_board(8)

    # Print initial board
    print("Initial Board:")
    print_board(board)

    # Solve the board based on the specified method
    if method == "annealing":
        solved_board, moves = simulated_annealing()
    elif method == "hill_climbing":
        solved_board, moves = hill_climbing_with_sidestep()
    else:
        print(f"Invalid method: {method}")
        return

    if solved_board:
        print("\nSolved Board after", moves, "moves:")
        print_board(solved_board)
    else:
        print("\nNo solution found after", moves, "moves.")

if __name__ == "__main__":
    testing_simulation(100)
    method_choice = "hill_climbing"
    solve_and_display(method_choice)
