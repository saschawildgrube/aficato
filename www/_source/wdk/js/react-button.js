'use strict';

class Button extends WDKReactComponent
{
	constructor(props)
	{
		super(props);
	}
	
	OnClick()
	{
		if (GetType(this.props.callbackOnClick) == 'function') 
		{
			this.props.callbackOnClick(this.props.value);
		}
	}
	
	render()
	{
		var strText = GetStringValue(this.props.text);
		if (strText != '')
		{
			var inner = strText;
		}
		else
		{
			var inner = this.props.children;
		}
		return e('button',
			{
				onClick: this.OnClick
			},
			inner);	
	}	
}