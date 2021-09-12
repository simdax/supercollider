using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Move : MonoBehaviour
{
    public GameObject other;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        float vertical = Input.GetAxis("Vertical") * 0.1f;
        float horizontal = Input.GetAxis("Horizontal") * 0.1f;
        transform.Translate(vertical, horizontal, 0);
        var dist = Mathf.Clamp(
            Vector3.Distance(other.transform.position, transform.position),
            0, 40
            ) / 40;
        Debug.Log(dist);
        Lang.msg("~playing.set(\\amp, " + dist + ")");
    }
}
