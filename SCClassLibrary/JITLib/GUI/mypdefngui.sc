PdefnGuiPlus : PdefnGui{
	*new{ arg ... args;
		^super.new(*args).changeSCV;
	}
	changeSCV{
		csView.action={
			arg self; object.source=self.value
		};
	}
}
