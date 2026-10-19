const maze = [
  "S O O X O O O O O O", "X X O X O X X X X O",
  "O O O O O O O X O O", "O X X X X X O X O X",
  "O O O O O O O O O X", "X X X X X X X X O X",
  "O O O O O O O O O X", "O X X X X X X X O X",
  "O O O O O O O O O G", "O O O O O O O O O O"
].map(row => row.split(" "));
const directions = [[-1, 0], [0, 1], [1, 0], [0, -1]];
const start = [0, 0], goal = [8, 9];
const rows = maze.length, columns = maze[0].length;
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
let snapshots = [], currentStep = 0, timer = null;

function inBounds([row, column]) { return row >= 0 && row < rows && column >= 0 && column < columns; }
function key([row, column]) { return `${row},${column}`; }
function addSnapshot(robot, message, visited, path = []) {
  snapshots.push({ robot: [...robot], message, visited: new Set(visited), path: path.map(cell => [...cell]) });
}
function dfs(cell, visited, parents) {
  if (!inBounds(cell) || maze[cell[0]][cell[1]] === "X" || visited.has(key(cell))) return false;
  visited.add(key(cell));
  addSnapshot(cell, "DFS visits this cell and marks it visited.", visited);
  if (key(cell) === key(goal)) {
    addSnapshot(cell, "Goal reached. DFS returns through the call stack.", visited);
    return true;
  }
  for (const [dr, dc] of directions) {
    const next = [cell[0] + dr, cell[1] + dc];
    if (!inBounds(next) || visited.has(key(next)) || maze[next[0]][next[1]] === "X") continue;
    addSnapshot(cell, `DFS moves forward to (${next[0]}, ${next[1]}).`, visited);
    parents.set(key(next), cell);
    if (dfs(next, visited, parents)) return true;
    addSnapshot(cell, "Dead end found; DFS backtracks to this cell.", visited);
  }
  return false;
}
function buildHistory() {
  const visited = new Set(), parents = new Map();
  addSnapshot(start, "Initial maze: DFS starts at S.", visited);
  const found = dfs(start, visited, parents);
  if (!found) {
    resultElement.textContent = "No path exists from S to G.";
    return;
  }
  const path = [];
  let cell = goal;
  while (cell) { path.unshift(cell); cell = parents.get(key(cell)); }
  addSnapshot(goal, `Maze solved: ${path.length - 1} move(s) in the DFS path.`, visited, path);
  resultElement.textContent = `DFS solution path: ${path.length - 1} moves`;
}
function render() {
  const snapshot = snapshots[currentStep];
  gridElement.innerHTML = "";
  maze.forEach((row, r) => row.forEach((value, c) => {
    const cell = document.createElement("div"), cellKey = `${r},${c}`;
    cell.className = "cell";
    if (snapshot.visited.has(cellKey)) cell.classList.add("visited");
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
function stopPlaying() { if (timer) clearInterval(timer); timer = null; playButton.textContent = "▶ Play"; }
function move(amount) {
  currentStep = Math.max(0, Math.min(snapshots.length - 1, currentStep + amount));
  render();
  if (currentStep === snapshots.length - 1) stopPlaying();
}
previousButton.addEventListener("click", () => move(-1));
nextButton.addEventListener("click", () => move(1));
resetButton.addEventListener("click", () => { stopPlaying(); currentStep = 0; render(); });
playButton.addEventListener("click", () => {
  if (timer) stopPlaying();
  else { playButton.textContent = "❚❚ Pause"; timer = setInterval(() => move(1), Number(speedInput.value)); }
});
buildHistory();
render();
