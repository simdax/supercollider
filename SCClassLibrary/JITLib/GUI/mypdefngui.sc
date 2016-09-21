PdefnGuiPlus : PdefnGui{

	var <quantView;
	
	makeViews {
		var lineheight = max(
			skin.buttonHeight * numItems + skin.headHeight,
			zone.bounds.height)  - (skin.margin.y * 2);

		nameView = DragBoth(zone, Rect(0,0, 60, lineheight))
		.font_(font)
		.align_(\center)
		.receiveDragHandler_({ arg obj; this.object = View.currentDrag });

		csView = EZText(zone,
			Rect(0,0, bounds.width - 95, lineheight),
			nil, { |ez| object.source = ez.value; })
		.font_(font);
		csView.bounds;

		quantView= EZNumber(zone, Rect(0,0, 30, lineheight),
			nil,[0,124,nil,1,4],
			{|source| object.quant=source.value}
		).font_(font)
		
	}

	checkUpdate {
		var newState = this.getState;

		var show = newState[\object].notNil;
		zone.visible_(show);
		if (show.not) {
			prevState = newState;
			^this
		};

		if (newState[\object] != prevState[\object]) {
			this.name_(this.getName);
		};
		// works with a little delay, but works
		if (newState[\source] != prevState[\source]) {
			defer { csView.textField.string_(object.source.asCode); };
		};
		if (newState[\quant] != prevState[\quant]) {
			defer { quantView.value_(object.quant); };
		};

		prevState = newState;
	}
}
