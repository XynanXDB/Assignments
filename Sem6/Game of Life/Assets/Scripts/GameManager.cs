using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.Linq;
public class GameManager : MonoBehaviour {

	private static GameManager _instance;
	public static GameManager Instance {
		get {
			if (_instance == null) {
				if (GameObject.Find ("GameManager") != null) {
					_instance = GameObject.Find ("GameManager").GetComponent<GameManager> ();
				}
			}
			return _instance;
		}
	}


	public static bool isRunning = false;
	public static bool isUpdate = false;
	public Cell cellPrefab;
	public static Cell[,] cellMatrix;
	public static List<Cell> activatedCells;
	public static int rowCount = 200;
	public static int colCount = 200;
	public static List<Cell> toBeResurrected;

	// Use this for initialization
	void Start () {
		activatedCells = new List<Cell> ();
		toBeResurrected = new List<Cell> ();
		SpawnCells ();
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetMouseButtonDown (0) && !ClickBlock.isOverUI) {
			int i = (int)Controls.MatchCoordinates ().y;
			int j = (int)Controls.MatchCoordinates ().x;

			ToggleCell (i, j);

		}
	}

	void SpawnCells () {
		cellMatrix = new Cell[rowCount, colCount];

		for (int i = 0; i < rowCount; i++) {
			for (int j = 0; j < colCount; j++) {
				
				float x = j - colCount / 2.0f + 0.5f;
				float y = i - rowCount / 2.0f + 0.5f;

				Cell cell = Instantiate (cellPrefab, new Vector3 (x, y, 0), Quaternion.identity) as Cell;
				cell.x = i;
				cell.y = j;
				cellMatrix [i, j] = cell;
			}
		}
	}

	public List<Cell> GetAdjacentCells (int i, int j) {
		//x Visualisation
		//x      07 00 01
		//x      06 xx 02
		//x      05 04 03

		List<Cell> adj = new List<Cell> ();
	



		//x 0
		if (i + 1 < rowCount && j < colCount && j > 0)
			adj.Add(cellMatrix [i + 1, j]);
		//x 1
		if (i + 1 < rowCount && j + 1 < colCount) 
			adj.Add (cellMatrix [i + 1, j + 1]);
		//x 2
		if (i < rowCount && i > 0 && j + 1 < colCount)
			adj.Add(cellMatrix [i    , j + 1]);
		//x 3
		if (i - 1 > 0 && j + 1 < colCount)
			adj.Add(cellMatrix [i - 1, j + 1]);
		//x 4
		if (i - 1 > 0 && j < colCount && j > 0)
			adj.Add(cellMatrix [i - 1, j    ]);
		//x 5
		if (i - 1 > 0 && j - 1 > 0)
			adj.Add(cellMatrix [i - 1, j - 1]);
		//x 6
		if (i < rowCount && i > 0 && j - 1 > 0)
			adj.Add(cellMatrix [i    , j - 1]);
		//x 7
		if (i + 1 < rowCount && j - 1 > 0)
			adj.Add(cellMatrix [i + 1, j - 1]);
		
		return adj;
	}

	void ToggleCell (int i, int j) {
		if (cellMatrix [i, j].condition == Cell.Condition.DEAD) {
			cellMatrix [i, j].condition = Cell.Condition.ALIVE;
			cellMatrix [i, j].neighbours = new List<Cell> ();
			cellMatrix [i, j].neighbours = GetAdjacentCells (i, j);
			cellMatrix [i, j].deadNeighbours = new List<Cell> ();
			cellMatrix [i, j].deadNeighboursSurroundings = new List<Cell> ();
			activatedCells.Add (cellMatrix [i, j]);

		} else if (cellMatrix [i, j].condition == Cell.Condition.ALIVE) {
			activatedCells.Remove (cellMatrix [i, j]);
			cellMatrix [i, j].condition = Cell.Condition.DEAD;
			cellMatrix [i, j].neighbours.Clear ();
			cellMatrix [i, j].deadNeighbours.Clear ();
			cellMatrix [i, j].deadNeighboursSurroundings.Clear ();
		}
		cellMatrix [i, j].ChangeColor ();
	}

	public void UpdateCell () {
		Iterate:
		activatedCells = activatedCells.OrderByDescending (x => x.AliveCellsGetSurrounding ()).ToList ();
		for (int a = 0; a < activatedCells.Count; a++) {
			
			for (int y = 0; y < activatedCells [a].neighbours.Count; y++) {
				#region Update Dead Neighbours
				if (activatedCells [a].neighbours [y].condition == Cell.Condition.DEAD) {
					for (int x = 0; x < activatedCells [a].deadNeighbours.Count; x++) {
						if (activatedCells [a].deadNeighbours [x] == activatedCells [a].neighbours [y] && activatedCells != null && activatedCells[a].neighbours[y] != null) {
							activatedCells [a].deadNeighbours.Remove (activatedCells [a].neighbours [y]);
						} else {
							activatedCells[a].deadNeighbours[x].aliveCellsAroundDeadCells = activatedCells[a].deadNeighbours[x].DeadCellsGetNextConditionOfSurrounding();
						}

					}
						
					activatedCells [a].deadNeighbours.Add (activatedCells [a].neighbours [y]);
					
					#endregion

					#region Update Dead Neighbour Surroundings
					for (int x = 0; x < activatedCells [a].deadNeighbours.Count; x++) {
						activatedCells [a].deadNeighbours [x].deadNeighboursSurroundings = GetAdjacentCells (activatedCells [a].deadNeighbours [x].x, activatedCells [a].deadNeighbours [x].y);
					}
					#endregion

				} else if (activatedCells[a].neighbours[y].condition == Cell.Condition.ALIVE) {
					activatedCells [a].deadNeighbours.Remove (activatedCells [a].neighbours [y]);
				}
			}

			if (activatedCells [a].condition == Cell.Condition.DEAD) {
				activatedCells.Remove (activatedCells[a]);
				goto Iterate;
			}
			activatedCells [a].UpdateCell ();
		}

		for (int a = 0; a < activatedCells.Count; a++) {
			if (isRunning) {
				activatedCells[a].condition = activatedCells[a].nextCondition;
				activatedCells[a].ChangeColor ();
			}
		}


		if (isRunning) {
			foreach (Cell b in toBeResurrected) {

				b.condition = b.nextCondition;
				b.ChangeColor ();
				b.neighbours = new List<Cell> ();
				b.neighbours = GetAdjacentCells (b.x, b.y);
				b.deadNeighbours = new List<Cell> ();
				b.deadNeighboursSurroundings = new List<Cell> ();
				activatedCells.Add (b);
			}
			toBeResurrected.Clear();
		}

	}

}
