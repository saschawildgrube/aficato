'use strict';

class AMLRedFlagsLegendReactComponent extends React.Component
{
  constructor(props)
  {
    super(props);
    this.state =
    {
    };
  }
  
  GetText(strTypologyID)
	{
		switch (strTypologyID)
		{
		case 'INACTIVITY':
			return '?TID_AML_TYPOLOGY_INACTIVITY?';
		case 'STRUCTURING':
			return '?TID_AML_TYPOLOGY_STRUCTURING?';
		case 'INTERNATIONALWIRES':
			return '?TID_AML_TYPOLOGY_INTERNATIONALWIRES?';
		case 'RAPIDMOVEMENT':
			return '?TID_AML_TYPOLOGY_RAPIDMOVEMENT?';
		case 'ROUNDAMOUNTS':
			return '?TID_AML_TYPOLOGY_ROUNDAMOUNTS?';
		case 'INTENSIVECASH':
			return '?TID_AML_TYPOLOGY_INTENSIVECASH?';
		case 'HIGHRISKGEOGRAPHIES':
			return '?TID_AML_TYPOLOGY_HIGHRISKGEOGRAPHIES?';
		default:
			return '';
		}
	}
	
	GetIcon(strTypologyID)
	{
		return 'check-square-o';
		switch (strTypologyID)
		{
		case 'INTENSIVECASH':
			return 'money';
		default:
			return '';
		}
	}

  render()
  {
		var aSupportedTypologyIDs = this.props.supportedtypologyids;
		var aDetectedTypologyIDs = this.props.detectedtypologyids;
	  var items = [];
		aSupportedTypologyIDs.map( (strTypologyID) =>
		{
			var bRaised = aDetectedTypologyIDs.includes(strTypologyID);	
		  items.push(
	  	{
		  	icon: (bRaised)?(this.GetIcon(strTypologyID)):('square-o'),
		  	iconcolor: (bRaised)?('red'):('black'),
		  	textcolor: (bRaised)?('red'):('black'),
		  	key: strTypologyID,
		  	text: this.GetText(strTypologyID)
	  	});
		});

	  var props = [];
	  props.items = items;
  	return e(IconLegendReactComponent,props,null);
  }
}

