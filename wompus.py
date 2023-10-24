import random as rd

class WumpusWorld:
    def __init__(self, size=4 ):
        self.size = size
        self.board = [[' ' for i in range(size)] for i in range(size)]
        self.player_pos = [0, self.size-1]
        self.arrow = 1
        self.alive = True
        self.has_gold = False
        self.score = 0
        self.directions = ["UP", "RIGHT", "DOWN", "LEFT"]
        self.direction_index = 0
        self.num_pits = 3

        self.place_item("W")
        self.place_item("G")
        for i in range(self.num_pits):
            self.place_item("P")
    
    def place_item(self, item):
        while True:
            x, y = rd.randint(0, self.size-1), rd.randint(0, self.size-1)
            if self.board[y][x] == " " and (x, y) != (0, 0):
                self.board[y][x] = item
                break

    def get_wumpus_position(self):
        for y in range(self.size):
            for x in range(self.size):
                if self.board[y][x] == "W":
                    return [x, y]
    
    def move_wumpus(self):
        wumpus_position = self.get_wumpus_position()
        if wumpus_position is None:  # Check if the Wumpus is dead (not on the board)
            return  # If the Wumpus is dead, just return without doing anything

        wumpus_x, wumpus_y = wumpus_position
        player_x, player_y = self.player_pos

        # Determine the best direction to move in order to get closer to the player
        if wumpus_x == player_x:
            dx = 0
        elif wumpus_x < player_x:
            dx = 1
        else:
            dx = -1

        if wumpus_y == player_y:
            dy = 0
        elif wumpus_y < player_y:
            dy = 1
        else:
            dy = -1

        new_x = wumpus_x + dx
        new_y = wumpus_y + dy

        # Move the Wumpus if the new position is valid
        if 0 <= new_x < self.size and 0 <= new_y < self.size and self.board[new_y][new_x] == " ":
            self.board[wumpus_y][wumpus_x] = " "
            self.board[new_y][new_x] = "W"
            
    def move(self):
        dy, dx = 0, 0
        direction = self.directions[self.direction_index]
        if direction == "UP":
            dy = -1
        elif direction == "DOWN":
            dy = 1
        elif direction == "LEFT":
            dx = -1
        elif direction == "RIGHT":
            dx = 1
                
        new_x = self.player_pos[0] + dx
        new_y = self.player_pos[1] + dy

        # Check if the new position is valid before moving
        if 0 <= new_x < self.size and 0 <= new_y < self.size:
            self.player_pos = [new_x, new_y]
            if self.board[new_y][new_x] == "W":
                print("You walked into the Wumpus!")
                self.score -= 1000
                self.alive = False
            elif self.board[new_y][new_x] == "P":
                print("You fell into a pit!")
                self.score -= 1000
                self.alive = False
        else:
            print("You bumped into a wall!")
        
    def is_alive(self):
        return self.alive
    
    def turn_left(self):
        self.direction_index = (self.direction_index - 1) % len(self.directions)

    def turn_right(self):
        self.direction_index = (self.direction_index + 1) % len(self.directions)

    def shoot(self):
        if self.arrow == 0:
            print("No arrows left!")
            return

        dx, dy = 0, 0
        direction = self.directions[self.direction_index]
        if direction == "UP":
            dy = -1
        elif direction == "DOWN":
            dy = 1
        elif direction == "LEFT":
            dx = -1
        elif direction == "RIGHT":
            dx = 1

        temp_x, temp_y = self.player_pos[0], self.player_pos[1]
        while 0 <= temp_x < self.size and 0 <= temp_y < self.size:
            if self.board[temp_y][temp_x] == "W":
                print("You killed the Wumpus!")
                self.board[temp_y][temp_x] = ' '
                break
            temp_x += dx
            temp_y += dy

        self.arrow -= 1
        self.score -= 10
    
    def grab(self):
        x, y = self.player_pos
        if self.board[y][x] == "G":
            self.has_gold = True
            self.board[y][x] = " "
            print("You have grabbed the gold!")
            print("Congratulations! You have safely returned with the gold and won the game!")
            self.alive = False

    def release(self): 
        if self.has_gold:
            x, y = self.player_pos
            self.board[y][x] = "G"
            self.has_gold = False
    
    def take_action(self, action):
        if action == "MOVE":
            self.move()
        elif action == "TURN_LEFT":
            self.turn_left()
        elif action == "TURN_RIGHT":
            self.turn_right()
        elif action == "SHOOT":
            self.shoot()
        elif action == "GRAB":
            self.grab()
        elif action == "RELEASE":
            self.release()
        self.score -= 1  # Decrement score for each action
        self.move_wumpus()
        
    def display(self):
        adjacents = self.get_adjacent_positions()

        for y in range(self.size):
            for x in range(self.size):
                if self.player_pos == [x, y]:
                    print('A', end=' ')
                elif [x, y] in adjacents:
                    # Print whatever is on the square (Wumpus, Pit, Gold, or just a dot if it's empty)
                    print(self.board[y][x] if self.board[y][x] != ' ' else '.', end=' ')
                else:
                    print('X', end=' ')  # Show 'X' for non-adjacent positions
            print()


    def get_front_position(self):
        x, y = self.player_pos
        direction = self.directions[self.direction_index]
        if direction == "UP" and y > 0:
            return [x, y-1]
        elif direction == "DOWN" and y < self.size-1:
            return [x, y+1]
        elif direction == "LEFT" and x > 0:
            return [x-1, y]
        elif direction == "RIGHT" and x < self.size-1:
            return [x+1, y]
        else:
            return self.player_pos  # Return current position if there's no valid front position

    
    def get_adjacent_positions(self):
        x, y = self.player_pos
        positions = []

        # Define possible movements based on current direction
        movements = {
            "UP": {"front": [x, y-1], "back": [x, y+1], "left": [x-1, y], "right": [x+1, y]},
            "DOWN": {"front": [x, y+1], "back": [x, y-1], "left": [x+1, y], "right": [x-1, y]},
            "LEFT": {"front": [x-1, y], "back": [x+1, y], "left": [x, y+1], "right": [x, y-1]},
            "RIGHT": {"front": [x+1, y], "back": [x-1, y], "left": [x, y-1], "right": [x, y+1]}
        }

        direction = self.directions[self.direction_index]
        for key, pos in movements[direction].items():
            if 0 <= pos[0] < self.size and 0 <= pos[1] < self.size:
                positions.append(pos)

        return positions


    def get_percept(self):
        x, y = self.player_pos
        percept = {
            'Stench': "W" in self.get_adjacent_squares(),
            'Breeze': "P" in self.get_adjacent_squares(),
            'Glitter': self.board[y][x] == 'G'
        }
        return percept
    
    def get_adjacent_squares(self):
        x,y = self.player_pos
        adjacents = []
        if x > 0:
            adjacents.append(self.board[y][x-1])
        if x < self.size - 1:
            adjacents.append(self.board[y][x+1])
        if y > 0: 
            adjacents.append(self.board[y-1][x])
        if y < self.size - 1:
            adjacents.append(self.board[y+1][x])
        return adjacents

class KBAgent:
    def __init__(self, world):
        self.world = world
        self.kb = []
        self.t = 0
        self.consecutive_turns = 0 
        self.stack = []
        self.agent_limit = 1000
        self.visited = []  # List to remember visited cells
        self.last_actions = []  # List to remember the last few actions
        
    def get_adjacent_positions(self, position=None):
        if not position:
            position = self.world.player_pos
        x, y = position
    def ask(self, query):
        # If the query is directly in the KB, return True
        if query in self.kb:
            return True

        # If there's a breeze at a certain position, infer the possibility of a pit in the adjacent squares
        if "Breeze" in query:
            position = self.extract_position_from_query(query) # function to extract the position from the query
            adjacents = self.world.get_adjacent_positions()
            for adj in adjacents:
                if f"Pit at {adj}" in self.kb:
                    return True

        # Add more inferences based on the type of query and the current state of the KB

        # If none of the above logic inferred the query to be true, return False
        return False

    def extract_position_from_query(self, query):
        # Extracts the position from a string like "Breeze at [1,2]"
        position_str = query.split("at")[-1].strip()
        x, y = map(int, position_str[1:-1].split(","))
        return [x, y]

    def tell(self, sentence):
        self.kb.append(sentence)
    
    def ask(self, query):
        return query in self.kb
    
    def make_percept_sentence(self, percept):
        return f"At time {self.t}, percept is {percept}"

    def make_action_query(self):
        adj_positions = self.world.get_adjacent_positions()
        
        # 1. If current position has gold
        if self.world.get_percept()['Glitter']:
            return "GRAB"
        
        # 2. If the current position is breezy or stenchy and unvisited, it's potentially unsafe.
        if (self.world.get_percept()['Breeze'] or self.world.get_percept()['Stench']) and tuple(self.world.player_pos) not in self.visited:
            self.tell(f"Unsafe at {self.world.player_pos}")

        # 3. If the current position is neither breezy nor stenchy, it's safe.
        if not self.world.get_percept()['Breeze'] and not self.world.get_percept()['Stench']:
            self.tell(f"Safe at {self.world.player_pos}")

        # 4. If there are safe adjacent unvisited positions, prioritize them.
        safe_adj_positions = [adj for adj in adj_positions if f"Safe at {adj}" in self.kb and adj not in self.visited]
        
        if safe_adj_positions:
            # Push current position to the stack
            self.stack.append(self.world.player_pos)
            next_pos = rd.choice(safe_adj_positions)
            direction_needed = self.get_direction_to_reach(next_pos)
            if direction_needed == self.world.directions[self.world.direction_index]:
                return "MOVE"
            else:
                return "TURN_LEFT" if (self.world.direction_index - self.world.directions.index(direction_needed)) % 4 < 2 else "TURN_RIGHT"

        # 5. If we're here, it means all adjacent positions are either unsafe or visited. Backtrack.
        while self.stack:
            backtrack_pos = self.stack.pop()
            if backtrack_pos not in self.visited:
                direction_needed = self.get_direction_to_reach(backtrack_pos)
                if direction_needed == self.world.directions[self.world.direction_index]:
                    return "MOVE"
                else:
                    return "TURN_LEFT" if (self.world.direction_index - self.world.directions.index(direction_needed)) % 4 < 2 else "TURN_RIGHT"

        # 6. If the stack is empty, take a calculated risk.
        unvisited_adj_positions = [adj for adj in adj_positions if adj not in self.visited]
        if unvisited_adj_positions:
            # This is a calculated risk: moving to an unvisited adjacent position even if it might be unsafe.
            next_pos = rd.choice(unvisited_adj_positions)
            direction_needed = self.get_direction_to_reach(next_pos)
            if direction_needed == self.world.directions[self.world.direction_index]:
                return "MOVE"
            else:
                return "TURN_LEFT" if (self.world.direction_index - self.world.directions.index(direction_needed)) % 4 < 2 else "TURN_RIGHT"
            
                # Improved Wumpus handling
        stenches = [loc for loc, percept in self.kb.items() if 'Stench' in percept and percept['Stench']]
        possible_wumpus_locs = []
        for loc in stenches:
            adjacents = self.get_adjacent_positions(loc)
            for adj in adjacents:
                if adj not in self.visited:
                    possible_wumpus_locs.append(adj)

        if possible_wumpus_locs and self.world.arrow:
            target = rd.choice(possible_wumpus_locs)
            direction_needed = self.get_direction_to_reach(target)
            if direction_needed == self.world.directions[self.world.direction_index]:
                return "SHOOT"
            else:
                return "TURN_LEFT" if (self.world.direction_index - self.world.directions.index(direction_needed)) % 4 < 2 else "TURN_RIGHT"


        if len(set(self.last_actions[-4:])) == 1 or len(self.last_actions) > 10:
            choice = rd.choice(["MOVE", "TURN_LEFT", "TURN_RIGHT", "MOVE"])
            self.last_actions.clear()  # Clear the actions log to reset any potential repetitive patterns.
            return choice
        # If we can't decide based on the above logic, turn randomly.
        return rd.choice(["TURN_LEFT", "TURN_RIGHT"])

    
    def get_direction_to_reach(self, target):
        x, y = self.world.player_pos
        tx, ty = target

        if tx == x:
            if ty < y:
                return "UP"
            else:
                return "DOWN"
        elif ty == y:
            if tx < x:
                return "LEFT"
            else:
                return "RIGHT"

    def make_action_sentence(self, action):
        return f"At time {self.t}, action taken is {action}"
    
    def agent_program(self):
        while self.world.is_alive() and self.t <= self.agent_limit:
            print(f"\n--- Time step: {self.t} ---")

            # Remember visited cells
            current_pos = tuple(self.world.player_pos)
            if current_pos not in self.visited:
                self.visited.append(current_pos)

            percept = self.world.get_percept()
            print(f"Percepts received: {percept}")

            self.tell(self.make_percept_sentence(percept))
            action = self.make_action_query()

            print(f"Based on knowledge base, deciding to: {action}")

            self.tell(self.make_action_sentence(action))
            self.last_actions.append(action)  # Store the action

            if action != "AVOID":
                self.world.take_action(action)

            self.world.display()

            direction = self.world.directions[self.world.direction_index]
            print(f"Agent is now facing: {direction}")

            self.t += 1

class UserAgent:
    def __init__(self, world):
        self.world = world
    
    def get_user_input(self):
        while True:
            print("\nChoose an action:")
            print("1: MOVE")
            print("2: TURN LEFT")
            print("3: TURN RIGHT")
            print("4: SHOOT (if you have arrows left)")
            print("5: GRAB")
            print("6: RELEASE")
            choice = input("> ")
            if choice == "1":
                return "MOVE"
            elif choice == "2":
                return "TURN_LEFT"
            elif choice == "3":
                return "TURN_RIGHT"
            elif choice == "4" and self.world.arrow > 0:
                return "SHOOT"
            elif choice == "5":
                return "GRAB"
            elif choice == "6":
                return "RELEASE"
            else:
                print("Invalid input. Please try again.")
        
    def agent_program(self):
        while self.world.is_alive():
            self.world.display()
            direction = self.world.directions[self.world.direction_index]
            print(f"\nYou are facing {direction}")
            percept = self.world.get_percept()
            print("Percepts:")
            for key, value in percept.items():
                print(f"{key}: {value}")
            action = self.get_user_input()
            self.world.take_action(action)

if __name__ == "__main__":
    world = WumpusWorld()
    agent = KBAgent(world)
    agent.agent_program()