using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Linq;

public enum HEURISTIC_TYPE {
	MANHATTAN = 0,
	EUCLIDEAN,
	OCTILE,
	CHEBYSHEV,

	TOTAL
}

public class GameManagerScript : MonoBehaviour {

	private static GameManagerScript mInstance;
	public static GameManagerScript Instance {
		get {
			if (mInstance == null) {
				if (GameObject.Find ("GameManager") != null) {
					mInstance = GameObject.Find ("GameManager").GetComponent<GameManagerScript> ();
				}
			}
			return mInstance;
		}
	}

	[HideInInspector] public const int rowCount = 200;
	[HideInInspector] public const int colCount = 200;
	public CellScript cellPrefab;
	public static CellScript[,] cellMatrix;
	public static List<CellScript> walls;
	public static List<CellScript> processList;
	public static List<CellScript> dumpList;
	public static CellScript[] path;
	public static bool isRun = false;
	public static bool isEndSearch = false;
	public static bool isAStar = true;
	public static bool isDiagonal = true;
	public int weight;
	int bufferI, bufferJ;
	static int generationCount;
	public LineRenderer lineRenderer;
	public Vector2 startPos = new Vector2 (101, 93);
	public Vector2 endPos = new Vector2 (101, 106);

	CellScript.CellType type = CellScript.CellType.TOTAL;
	public static HEURISTIC_TYPE heuristicType;

	// Use this for initialization
	public void Start () {
		walls = new List<CellScript> ();
		processList = new List<CellScript> ();
		dumpList = new List<CellScript> ();

		heuristicType = HEURISTIC_TYPE.MANHATTAN;
		lineRenderer = GetComponent<LineRenderer> ();

		SpawnCells ();
		InitStartEndCells ();
	}
	
	// Update is called once per frame
	void Update () {
		Debug.Log (startPos);
		if (!ClickBlock.isOverUI) {
			
			int i = (int)ControlsScript.MatchCoordinates ().y;
			int j = (int)ControlsScript.MatchCoordinates ().x;

			if (Input.GetMouseButtonDown (0)) {
				if (cellMatrix [i, j].type == CellScript.CellType.STARTPOINT) {
					type = CellScript.CellType.STARTPOINT;
				} else if (cellMatrix [i, j].type == CellScript.CellType.ENDPOINT) {
					type = CellScript.CellType.ENDPOINT;
				} else if (cellMatrix [i, j].type == CellScript.CellType.NORMAL) {
					type = CellScript.CellType.NORMAL;
				} else if (cellMatrix [i, j].type == CellScript.CellType.WALL) {
					type = CellScript.CellType.WALL;
				}
			}



			if (Input.GetMouseButton (0)) {

				if (bufferI != i || bufferJ != j) {
					ToggleCells (i, j);
					bufferI = i;
					bufferJ = j;
				} else {
					bufferI = i;
					bufferJ = j;
				}
			}
		}
	}

	void SpawnCells () {
		cellMatrix = new CellScript[rowCount, colCount];

		for (int i = 0; i < rowCount; i++) {
			for (int j = 0; j < colCount; j++) {
				
				float x = j - colCount / 2.0f + 0.5f;
				float y = i - rowCount / 2.0f + 0.5f;

				CellScript cell = Instantiate (cellPrefab, new Vector3 (x, y, 0), Quaternion.identity) as CellScript;
				cell.x = i;
				cell.y = j;
				cellMatrix [i, j] = cell;
			}
		}
	}

	void ToggleCells (int i , int j) {
		if (type == CellScript.CellType.NORMAL && cellMatrix [i, j].type == CellScript.CellType.NORMAL) {
			
			cellMatrix [i, j].type = CellScript.CellType.WALL;
			cellMatrix [i, j].neighbours = new List<CellScript> ();
			cellMatrix [i, j].renderer.color = Color.grey;
			walls.Add (cellMatrix [i, j]);

		} else if (type == CellScript.CellType.WALL && cellMatrix [i, j].type == CellScript.CellType.WALL) {
			
			cellMatrix [i, j].type = CellScript.CellType.NORMAL;
			cellMatrix [i, j].renderer.color = Color.white;
			walls.Remove (cellMatrix [i, j]);

		} else if (type == CellScript.CellType.STARTPOINT &&
		           cellMatrix [i, j].type != CellScript.CellType.ENDPOINT &&
		           cellMatrix [i, j].type != CellScript.CellType.WALL) {

			cellMatrix [i, j].type = CellScript.CellType.STARTPOINT;
			cellMatrix [i, j].renderer.color = Color.green;
			cellMatrix [i, j].nextCondition = CellScript.Condition.SETTLED;

			cellMatrix [(int)startPos.x, (int)startPos.y].type = CellScript.CellType.NORMAL;
			cellMatrix [(int)startPos.x, (int)startPos.y].renderer.color = Color.white;
			cellMatrix [(int)startPos.x, (int)startPos.y].nextCondition = CellScript.Condition.UNSETTLED;

			startPos.x = i;
			startPos.y = j;

		} else if (type == CellScript.CellType.ENDPOINT &&
		           cellMatrix [i, j].type != CellScript.CellType.STARTPOINT &&
		           cellMatrix [i, j].type != CellScript.CellType.WALL) {

			cellMatrix [i, j].type = CellScript.CellType.ENDPOINT;
			cellMatrix [i, j].renderer.color = Color.red;
			cellMatrix [i, j].nextCondition = CellScript.Condition.SETTLED;

			cellMatrix [(int)endPos.x, (int)endPos.y].type = CellScript.CellType.NORMAL;
			cellMatrix [(int)endPos.x, (int)endPos.y].renderer.color = Color.white;
			cellMatrix [(int)endPos.x, (int)endPos.y].nextCondition = CellScript.Condition.UNSETTLED;

			endPos.x = i;
			endPos.y = j;
		}
	}

	public void InitStartEndCells () {

		cellMatrix [(int)startPos.x, (int)startPos.y].type = CellScript.CellType.STARTPOINT;
		cellMatrix [(int)startPos.x, (int)startPos.y].renderer.color = Color.green;
		cellMatrix [(int)startPos.x, (int)startPos.y].nextCondition = CellScript.Condition.SETTLED;

		cellMatrix [(int)endPos.x, (int)endPos.y].type = CellScript.CellType.ENDPOINT;
		cellMatrix [(int)endPos.x, (int)endPos.y].renderer.color = Color.red;
		cellMatrix [(int)endPos.x, (int)endPos.y].nextCondition = CellScript.Condition.SETTLED;
	}

	public List<CellScript> GetAdjacentCell (int i, int j) {
		List<CellScript> adj = new List<CellScript> ();

		if (i + 1 < rowCount && j < colCount && j > 0) {	//x Up
			if (cellMatrix [i + 1, j].type != CellScript.CellType.WALL) {
				adj.Add (cellMatrix [i + 1, j]);
			}
		}

		if (i - 1 > 0 && j < colCount && j > 0) {	//x Down
			if (cellMatrix [i - 1, j].type != CellScript.CellType.WALL) {
				adj.Add (cellMatrix [i - 1, j]); 
			}
		}

		if (i < rowCount && i > 0 && j + 1 < colCount) {	//x Right
			if (cellMatrix [i, j + 1].type != CellScript.CellType.WALL) {
				adj.Add (cellMatrix [i, j + 1]); 
			}
		}

		if (i < rowCount && i > 0 && j - 1 > 0) {	//x Left
			if (cellMatrix [i, j - 1].type != CellScript.CellType.WALL) {
				adj.Add (cellMatrix [i, j - 1]); 
			}
		}

		//x Diagonals
		if (isDiagonal) {
			if (i + 1 < rowCount && j + 1 < colCount) { //x North East
				if (cellMatrix [i + 1, j].type != CellScript.CellType.WALL && cellMatrix [i, j + 1].type != CellScript.CellType.WALL && cellMatrix [i + 1, j + 1].type != CellScript.CellType.WALL) {
					adj.Add (cellMatrix [i + 1, j + 1]);
				}
			}

			if (i - 1 > 0 && j + 1 < colCount) { //x South East
				if (cellMatrix [i - 1, j].type != CellScript.CellType.WALL && cellMatrix [i, j + 1].type != CellScript.CellType.WALL && cellMatrix [i - 1, j + 1].type != CellScript.CellType.WALL) {
					adj.Add (cellMatrix [i - 1, j + 1]);
				}
			}

			if (i - 1 > 0 && j - 1 > 0) { //x South West
				if (cellMatrix [i - 1, j].type != CellScript.CellType.WALL && cellMatrix [i, j - 1].type != CellScript.CellType.WALL && cellMatrix [i - 1, j - 1].type != CellScript.CellType.WALL) {
					adj.Add (cellMatrix [i - 1, j - 1]);
				}
			}

			if (i + 1 < rowCount && j - 1 > 0) { //x North West
				if (cellMatrix [i + 1, j].type != CellScript.CellType.WALL && cellMatrix [i, j - 1].type != CellScript.CellType.WALL && cellMatrix [i + 1, j - 1].type != CellScript.CellType.WALL) {
					adj.Add (cellMatrix [i + 1, j - 1]);
				}
			}
		}
		return adj;
	}

	CellScript GetLowestDistanceTile () {
		CellScript lowestDistanceTile = processList [0];

		foreach (CellScript a in processList) {
			if (a.f < lowestDistanceTile.f) {
				lowestDistanceTile = a;
			}
		}

		lowestDistanceTile.neighbours = GetAdjacentCell (lowestDistanceTile.x, lowestDistanceTile.y);

		if (lowestDistanceTile.type == CellScript.CellType.STARTPOINT) {
			lowestDistanceTile.condition = lowestDistanceTile.nextCondition;

			foreach (CellScript a in lowestDistanceTile.neighbours) {
				if (a.h < lowestDistanceTile.h && !lowestDistanceTile.isClose) {
					lowestDistanceTile = a;
				}
			}
		}
		lowestDistanceTile.isClose = true;
		return lowestDistanceTile;
	}

	public void UpdateProcessList () { 

		CellScript lowestDistanceTile = GetLowestDistanceTile ();
		if (lowestDistanceTile.type == CellScript.CellType.ENDPOINT && lowestDistanceTile.condition == CellScript.Condition.SETTLED){
			isEndSearch = true;
			return ;
		}

		for (int j = 0; j < lowestDistanceTile.neighbours.Count; j++) {

			if (lowestDistanceTile.neighbours [j].isClose) {
				continue;
			}

			if (lowestDistanceTile.neighbours [j].type != CellScript.CellType.WALL) {

				lowestDistanceTile.neighbours [j].nextCondition = CellScript.Condition.SETTLED;

				int g = 0;

				if (lowestDistanceTile.neighbours [j].x == lowestDistanceTile.x || lowestDistanceTile.neighbours [j].y == lowestDistanceTile.y) {
					g = lowestDistanceTile.g + 10;
				} else {
					g = lowestDistanceTile.g + 14;
				}

				if (!lowestDistanceTile.neighbours [j].isOpen || g < lowestDistanceTile.neighbours [j].g) {
					lowestDistanceTile.neighbours [j].g = g;

					if (isAStar) {
						if (weight == 0)	weight = 1;
						lowestDistanceTile.neighbours [j].h = weight * HeuristicFunction (lowestDistanceTile.neighbours [j]);
						lowestDistanceTile.neighbours [j].f = lowestDistanceTile.neighbours [j].h + lowestDistanceTile.neighbours [j].g;
						lowestDistanceTile.neighbours [j].transform.SetParent (lowestDistanceTile.gameObject.transform);

						if (!lowestDistanceTile.neighbours [j].isOpen) {
							lowestDistanceTile.neighbours [j].renderer.color = Color.blue;
							lowestDistanceTile.neighbours [j].isOpen = true;
							processList.Add (lowestDistanceTile.neighbours [j]);
						}
					} else {
						lowestDistanceTile.neighbours [j].f = g;
						processList.Add (lowestDistanceTile.neighbours [j]);
						lowestDistanceTile.neighbours [j].renderer.color = Color.blue;
						lowestDistanceTile.neighbours [j].isOpen = true;
					}	
				}
			}
		}

		processList.Remove (lowestDistanceTile);
		dumpList.Add (lowestDistanceTile);
	}

	public void ApplyUpdateOnProcessList () {

		if (!isEndSearch) {
			
			for (int i = 0; i < processList.Count; i++) {
				processList [i].UpdateCell ();
			}

		} else {
			path = cellMatrix [(int)endPos.x, (int)endPos.y].GetComponentsInParent<CellScript> ();
			drawPath ();
		}
	}


	void drawPath (){
		lineRenderer.positionCount = path.Length;

		for (int i = 0; i < lineRenderer.positionCount; i++) {
			lineRenderer.SetPosition (i, path[i].transform.position + Vector3.back);
		}
	}

	int HeuristicFunction (CellScript cell) {
		int result = 0;

		if (heuristicType == HEURISTIC_TYPE.MANHATTAN && isDiagonal) {
			heuristicType = HEURISTIC_TYPE.OCTILE;
		} else if (heuristicType == HEURISTIC_TYPE.MANHATTAN && !isDiagonal) {
			heuristicType = HEURISTIC_TYPE.MANHATTAN;
		}

		int x = (int)Mathf.Abs ((int)endPos.x - (int)cell.x);
		int y = (int)Mathf.Abs ((int)endPos.y - (int)cell.y);

		switch (heuristicType) {

		case HEURISTIC_TYPE.MANHATTAN:
			result = 10 * (x + y);
		break;

		case HEURISTIC_TYPE.EUCLIDEAN:
			result = 10 * (int)Mathf.Sqrt (x * x + y * y);
		break;

		case HEURISTIC_TYPE.OCTILE:
			if (x > y) 
				result = 10 * (x - y) + 14 * y; 
			else 
				result = 10 * (y - x) + 14 * x;
		break;
		
		case HEURISTIC_TYPE.CHEBYSHEV:
			result = 10 * (int)Mathf.Max (x, y);
		break;
		}

		return result;
	}
}
