'use strict';

class InfoMessage extends WDKReactComponent
{
	constructor(props)
	{
		super(props);
	}

	render()
	{
		var strText = GetStringValue(this.props.text);
		if (strText == '')
		{
			return e('div',{});
		}
		return e('div',
			{
				className: 'alert alert-info'
			},
			strText);
	}	
}
