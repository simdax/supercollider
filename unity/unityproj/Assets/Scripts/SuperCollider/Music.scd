(
var c = ~sounds.music;
~playing  = Synth(c.instrument, [bufnum:c.bufnum, out:0, sustainTime:4000]);
)