const initialGrid = [
  "S O O X O O O O O O",
  "X X O X O X X X X O",
  "O O O O O O O X O O",
  "O X X X X X O X O X",
  "O O O O O O O O O X",
  "X X X X X X X X O X",
  "O O O O O O O O O X",
  "O X X X X X X X O X",
  "O O O O O O O O O G",
  "O O O O O O O O O O"
].map(row => row.split(" "));

const directions = [[-1, 0], [0, 1], [1, 0], [0, -1]];
const start = [0, 0];
const goal = [8, 9];
const rows = initialGrid.length;
const columns = initialGrid[0].length;
const gridElement = document.querySelector("#grid");
const messageElement = document.querySelector("#message");
const stepLabel = document.querySelector("#step-label");
const positionLabel = document.querySelector("#position-label");
const resultElement = document.querySelector("#result");
const previousButton = document.querySelector("#previous");
const nextButton = document.querySelector("#next");
const playButton = document.querySelector("#play");
const resetButton = document.querySelector("#reset");
const speedInput = document.querySelector("#speed");

let snapshots = [];
let currentStep = 0;
let timer = null;

function inBounds(row, column) {
  return row >= 0 && row < rows && column >= 0 && column < columns;
}

function copyGrid(grid) {
  return grid.map(row => [...row]);
}

function addSnapshot(grid, robot, message, visited, path = []) {
  snapshots.push({ grid: copyGrid(grid), robot, message, visited: new Set(visited), path });
}

function buildSearchHistory() {
  const queue = [[...start]];
  const visited = new Set(["0,0"]);
  const parents = new Map();
  const grid = copyGrid(initialGrid);
  addSnapshot(grid, start, "Initial state: S is added to the BFS queue.", visited);
  let found = false;

  while (queue.length) {
    const current = queue.shift();
    addSnapshot(grid, current, `Exploring cell (${current[0]}, ${current[1]}), the oldest queue item.`, visited);
    if (current[0] === goal[0] && current[1] === goal[1]) {
      found = true;
      addSnapshot(grid, current, "Goal reached. BFS stops because this is the shortest path.", visited);
      break;
    }
    for (const [dr, dc] of directions) {
      const next = [current[0] + dr, current[1] + dc];
      const key = `${next[0]},${next[1]}`;
      if (!inBounds(...next) || visited.has(key) || grid[next[0]][next[1]] === "X") continue;
      visited.add(key);
      parents.set(key, current);
      queue.push(next);
      addSnapshot(grid, next, `Discovered (${next[0]}, ${next[1]}) and added it to the queue.`, visited);
    }
  }

  if (found) {
    const path = [];
    let cell = goal;
    while (cell) {
      path.unshift(cell);
      cell = parents.get(`${cell[0]},${cell[1]}`);
    }
    addSnapshot(grid, goal, `Shortest path found: ${path.length - 1} move(s).`, visited, path);
    resultElement.textContent = `Shortest path: ${path.length - 1} moves`;
  } else {
    resultElement.textContent = "No path exists from S to G.";
  }
}

function render() {
  const snapshot = snapshots[currentStep];
  gridElement.innerHTML = "";
  snapshot.grid.forEach((row, r) => row.forEach((value, c) => {
    const cell = document.createElement("div");
    const key = `${r},${c}`;
    cell.className = "cell";
    if (snapshot.visited.has(key)) cell.classList.add("visited");
    if (value === "X") cell.classList.add("obstacle");
    if (value === "S") cell.classList.add("start");
    if (value === "G") cell.classList.add("goal");
    if (snapshot.path.some(([pr, pc]) => pr === r && pc === c)) cell.classList.add("path");
    if (snapshot.robot[0] === r && snapshot.robot[1] === c) cell.classList.add("robot");
    cell.textContent = value === "O" ? "" : value;
    cell.setAttribute("role", "gridcell");
    gridElement.appendChild(cell);
  }));
  stepLabel.textContent = `Step ${currentStep} / ${snapshots.length - 1}`;
  positionLabel.textContent = `Robot: (${snapshot.robot[0]}, ${snapshot.robot[1]})`;
  messageElement.textContent = snapshot.message;
  previousButton.disabled = currentStep === 0;
  nextButton.disabled = currentStep === snapshots.length - 1;
}

function stopPlaying() {
  if (timer) clearInterval(timer);
  timer = null;
  playButton.textContent = "▶ Play";
}

function move(step) {
  currentStep = Math.max(0, Math.min(snapshots.length - 1, currentStep + step));
  render();
  if (currentStep === snapshots.length - 1) stopPlaying();
}

previousButton.addEventListener("click", () => move(-1));
nextButton.addEventListener("click", () => move(1));
resetButton.addEventListener("click", () => { stopPlaying(); currentStep = 0; render(); });
playButton.addEventListener("click", () => {
  if (timer) {
    stopPlaying();
  } else {
    playButton.textContent = "❚❚ Pause";
    timer = setInterval(() => move(1), Number(speedInput.value));
  }
});

buildSearchHistory();
render();
