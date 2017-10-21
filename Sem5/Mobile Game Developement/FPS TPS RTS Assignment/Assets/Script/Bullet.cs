using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Bullet : MonoBehaviour {

	float speed = 10.0f;
	float blastRadius = 3.0f;
	float flyDistance = 20.0f;
	bool fly = false;
	Collider col;
	GameObject muzzleFlash;
	Vector3 initPos;
	public ParticleSystem particles;
	// Use this for initialization
	void Start () {
		muzzleFlash = transform.Find ("RPGMuzzleFlash").gameObject;
		muzzleFlash.SetActive (false);
		initPos = transform.position;
		col = this.transform.GetComponent<Collider> ();
	}
	
	// Update is called once per frame
	void Update () {
		if (ShootButton.Instance.isShot) {
			this.transform.parent = null;
			fly = true;
		}

		if (fly == true) {
			StartCoroutine (Move ());
		}

		Vector3 deltaPos = transform.position - initPos;
		if (deltaPos.z > flyDistance) {
			Destroy (this.gameObject);
		}
	}

	IEnumerator Move () {
		muzzleFlash.SetActive (true);
		this.transform.position += transform.right * Time.deltaTime * speed;
		yield return null;
	}

	void OnCollisionEnter (Collision other) {
		if (other.gameObject.tag == "Enemy") {

			RaycastHit[] hit;
			Vector3 origin = transform.position;

			hit = Physics.SphereCastAll (origin, blastRadius, transform.forward);

			foreach (RaycastHit a in hit) {
				if (a.collider.tag == "Enemy") {
					
					a.collider.gameObject.SetActive (false);

					ContactPoint contact;
					contact = other.contacts [0];
					Instantiate (particles, contact.point, Quaternion.identity);
					particles.Play ();
					particles.transform.parent = null;
					Destroy (this.gameObject);

				} 
			}
		}

		if (other.gameObject.tag == "Plane") {

			RaycastHit hit;
			Vector3 origin = transform.position;

			if (Physics.Raycast (origin, transform.right, out hit)) {
				ContactPoint contact;
				contact = other.contacts [0];
				Instantiate (particles, contact.point, Quaternion.identity);
				particles.Play ();
				particles.transform.parent = null;
				Destroy (this.gameObject);
			}
		}
	}
}
