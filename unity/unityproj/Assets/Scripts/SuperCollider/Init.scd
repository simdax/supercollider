// ~root = "C:/Users/scornaz/unityproj/Assets/Scenes/SuperCollider";

(
~root = "C:/Users/scornaz/unityproj/".standardizePath;

~sounds =
(
music: "Assets/Audio/2D MUSIC LOOP.wav"
).collect{arg value, i;
	SoundFile(~root ++ value).cue;
};
)
