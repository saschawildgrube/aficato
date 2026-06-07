// WDK and React
'use strict';

const e = React.createElement;

class WDKReactComponent extends React.Component
{
  constructor(props)
  {
    super(props);
    this.m_bIsMounted = false;
  	this.m_interval = null;
  	if (this.props.id != null)
  	{
  		this.m_strId = this.props.id;
  	}
  	else
  	{
  		this.m_strId = 'react_'+GetRandomToken(5); 
  	}
  	BindAllFunctionsToThis(this);
  }
  
  componentDidMount()
  {
  	this.m_bIsMounted = true;  
  }

  componentWillUnmount()
  {
  	this.m_bIsMounted = false;
  }
  
	componentDidUpdate(prevProps)
 	{
	}  
  
  Log(vValue)
  {
  	Trace(vValue);
  }
  
  IsMounted()
  {
  	return this.m_bIsMounted;	
  }

  
  StartTimer(nMilliseconds = 1000)
	{
		if (this.m_interval != null)
		{
			this.StopTimer();	
		}
		nMilliseconds = GetIntegerValue(nMilliseconds);
		this.m_interval = setInterval(() => { this.OnTimer()},nMilliseconds);
	}
	
	StopTimer()
	{
		if (this.m_interval != null)
		{
			clearInterval(this.m_interval);
			this.m_interval = null;	
		}
	}
	
	IsTimerActive()
	{
		if (this.m_interval != null)
		{
			return true;
		}
		return false;
	}

	OnTimer()
	{
		//Trace('OnTimer'); 
	}
	
	GetID()
	{
		return this.m_strId;
	}

}