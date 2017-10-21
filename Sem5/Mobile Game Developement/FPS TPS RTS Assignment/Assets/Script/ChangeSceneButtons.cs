using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class ChangeSceneButtons : MonoBehaviour {

	public void OnClickFPS() {
		SceneManager.LoadScene ("FPS");
	}

	public void OnClickTPS() {
		SceneManager.LoadScene ("TPS");
	}

	public void OnClickRTS() {
		SceneManager.LoadScene ("RTS");
	}

	public static string GetActiveScene () {
		return (SceneManager.GetActiveScene ().name);
	}
}
