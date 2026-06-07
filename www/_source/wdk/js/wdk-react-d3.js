// WDK D3ReactComponent
'use strict';

class D3ReactComponent extends WDKReactComponent
{
  componentDidMount()
  {
  	this.RenderD3('#'+this.GetID(), this.props);
  }

  render()
  {
  	var nHeight = 800;
  	var nWidth = 800;
  	if (this.props['height'] > 0)
  	{
  		nHeight = this.props['height'];
  	}
   	if (this.props['width'] > 0)
  	{
  		nWidth = this.props['width'];
  	}
  	return e(
  		'svg',
  		{
  			id: this.GetID(),
  			height: nHeight,
  			width: nWidth
  		},
  		null);
  }
  
  RenderD3(vSelector,aProps)
  {
  	Error('RenderD3 is NOT implemented!');	
  }
}

