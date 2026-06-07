'use strict';

class ButtonSelection extends WDKReactComponent
{
	constructor(props)
	{
		super(props);
	}
	
	OnClick(vValue)
	{
		if (GetType(this.props.callbackOnClick) == 'function')
		{
			this.props.callbackOnClick(vValue);
		}
	}
	
	render()
	{
		var aItems = GetArrayValue(this.props.items);
		var aComponents = [];
		var nKey = 0;
		for (var nItem = 0; nItem < aItems.length; nItem++)
		{
			if (nItem > 0)
			{
				aComponents.push(e('br',{key: nKey++ })); 
				aComponents.push(e('br',{key: nKey++ })); 
			}
			var strLabel = GetStringValue(GetValue(aItems,nItem,'label'));
			var vValue = GetValue(aItems,nItem,'value');
			aComponents.push(e(Button,
				{
					callbackOnClick: this.OnClick,
					value: vValue,
					key: nKey++
				},
				strLabel));
		}
		return e('div',{},aComponents);
	}	
}