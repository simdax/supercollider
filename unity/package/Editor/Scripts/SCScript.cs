using UnityEngine;

public class SCScript: MonoBehaviour
{
    public TextAsset script;

    public void Start() {
        Debug.Log("playing :" + script.text);
        Lang.msg(script); 
    }
}
 