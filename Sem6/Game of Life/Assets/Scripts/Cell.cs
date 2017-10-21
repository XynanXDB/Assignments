using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

public class Cell : MonoBehaviour {

	public enum Condition {
		ALIVE = 0,
		DEAD,

		TOTAL
	}

	public int x;
	public int y;
	public GameManager game;
	public List<Cell> neighbours;
	public List<Cell> deadNeighbours;
	public List<Cell> deadNeighboursSurroundings;
	public Condition condition;
	public Condition nextCondition;
	public int aliveCellsAroundActivatedCells;
	public int aliveCellsAroundDeadCells; 
	SpriteRenderer renderer;

	void Awake () {
		renderer = GetComponent<SpriteRenderer> ();
		nextCondition = Condition.DEAD;
	}
		

	public void UpdateCell () {
		nextCondition = condition;

		#region Update for Living Cells;
		aliveCellsAroundActivatedCells = AliveCellsGetSurrounding();

		if (condition == Condition.ALIVE) {

			if (aliveCellsAroundActivatedCells == 2){
				nextCondition = Condition.ALIVE;
			} else if (aliveCellsAroundActivatedCells < 2 || aliveCellsAroundActivatedCells > 3){
				nextCondition = Condition.DEAD;
			}
		#endregion

			#region Update For Dead Cells
			for (int i = 0; i < deadNeighbours.Count; i++){
				//x Current Condition Loop
				int count = 0;

				for (int j = 0; j < deadNeighbours[i].deadNeighboursSurroundings.Count; j++){
					if (deadNeighbours[i].deadNeighboursSurroundings[j].condition == Condition.ALIVE){
						count ++;
					}
				}
					
				if (count == 3) {
					deadNeighbours[i].nextCondition = Condition.ALIVE;
					for (int k = 0; k < GameManager.toBeResurrected.Count; k++){
						if (GameManager.toBeResurrected != null && GameManager.toBeResurrected[k] != deadNeighbours[i]){
								GameManager.toBeResurrected.Remove(deadNeighbours[i]);
						}
					}
					GameManager.toBeResurrected.Add(deadNeighbours[i]);
				} else if (count > 3){
					deadNeighbours[i].nextCondition = Condition.DEAD;
				} 


				if (deadNeighbours[i].condition == Condition.ALIVE) {
					deadNeighbours[i].aliveCellsAroundActivatedCells = deadNeighbours[i].AliveCellsGetSurrounding();
				}
			}
			#endregion 
		}
	}

	public void ChangeColor () {

		if (condition == Condition.ALIVE) {
			renderer.color = Color.green;
		} else {
			renderer.color = Color.white;
		}

		for (int i = 0; i < deadNeighbours.Count; i++) {
			if (deadNeighbours [i].condition == Condition.ALIVE) {
				deadNeighbours [i].renderer.color = Color.green;
			} else {
				deadNeighbours [i].renderer.color = Color.white;
			}
		}
	}

	public int AliveCellsGetSurrounding () {
		aliveCellsAroundActivatedCells = 0;

		for (int x = 0; x < neighbours.Count; x++) {
			if (neighbours[x].condition == Condition.ALIVE) {
				aliveCellsAroundActivatedCells++;
			}
		}
		return aliveCellsAroundActivatedCells;
	}

	public int DeadCellsGetNextConditionOfSurrounding () {
		int cells = 0;

		for (int x = 0; x < deadNeighboursSurroundings.Count; x++) {
			if (deadNeighboursSurroundings [x].nextCondition == Condition.ALIVE) {
				cells++;
			}
		}
		return cells;
	}
}
