'use strict';

class ColorLegendReactComponent extends React.Component
{
  constructor(props)
  {
    super(props);
  }

  render()
  {
		var aComponents = [];
		var aColorTextCombos = [];

	  for (let i = 0; i < this.props.items.length; i++)
	  {
	    const item = this.props.items[i];
	    const strColorTextCombo = item.text+item.color;
	    if (aColorTextCombos.find( (str) => { return str == strColorTextCombo; }) == null)
	    {
	    	aColorTextCombos.push(strColorTextCombo);
		    var reDivColor = e('div',
		    	{
		    		style:
		    		{
		    			float: 'left',
			    		backgroundColor: item.color,
		    			width: '20px',
		    			height: '20px'
		    		}
		    	},
		    	' ');
		    var reDivText = e('div',
		    	{
		    		style:
		    		{
		    			display: 'inline',
		    			paddingLeft: '5px'
		    		}
		    	},
		    	item.text);
	
		    aComponents.push(e(
		    	'div',
		    	{
		    		style: 
		    		{
		    			paddingBottom: '5px',
		    			paddingRight: '10px',
		    			float: 'left'
		    		},
		    		key: item.key	
		    	},
		    		reDivColor,
		    		reDivText));
		    	
		  }
		}

    aComponents.push(e(
    	'div',
    	{
    		style: 
    		{
    			clear: 'both'
    		},
    		key: 'clear-both'
    	}));



  	return e(
  		'div',
  		null,
  		aComponents);

  }
}

