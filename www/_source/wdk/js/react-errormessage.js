'use strict';

class ErrorMessage extends WDKReactComponent
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
				style: {
					color: 'red',
					borderColor: 'red',
					borderStyle: 'solid',
					borderWidth: 'thick',
					padding: '2px'
				}
			},
			strText);
	}	
}
