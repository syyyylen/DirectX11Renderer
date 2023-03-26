float4 vsmain( float4 pos : POSITION ) : SV_POSITION
{
	if(pos.y > 0 && pos.y < 1)
	{
		pos.x += 0.25f;
	}

	return pos;
}