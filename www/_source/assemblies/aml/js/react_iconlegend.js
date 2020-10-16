'use strict';

class IconLegendReactComponent extends React.Component
{
  constructor(props)
  {
    super(props);
  }

  render()
  {
		var aComponents = [];

	  for (let i = 0; i < this.props.items.length; i++)
	  {
	    const item = this.props.items[i];

	    var reDivIcon = e('i',
	    	{
	    		className: 'fa fa-'+item.icon,
	    		style:
	    		{
		    		color: item.iconcolor
	    		}
	    	},
	    	' ');
	    var reDivText = e('div',
	    	{
	    		style:
	    		{
	    			display: 'inline',
	    			paddingLeft: '5px',
	    			color: item.textcolor,
	    			fontSize: '18px'
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
	    		reDivIcon,
	    		reDivText));
		    	
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

