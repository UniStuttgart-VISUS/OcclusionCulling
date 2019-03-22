#version 110

layout (local_size_x = 32, local_size_y = 32) in;



bool IsVisible() {
	uint ii;
	vec3 absHalf = abs3(half);
	
	float3 planeToPoint = center - mpPosition[0]; // Use near-clip-top-left point for point on first three planes
	for( ii=0; ii<3; ii++ )
	{
	    float3 normal      = mpNormal[ii];
	    float3 absNormal   = abs3(normal);
	    float  nDotC       = dot3( normal, planeToPoint );
	    if( nDotC > dot3( abs3(normal), absHalf ) )
	    {
	        return false;
	    }
	}
	
	planeToPoint = center - mpPosition[6]; // Use far-clip-bottom-right point for point on first three planes
	for( ii=3; ii<6; ii++ )
	{
	    float3 normal      = mpNormal[ii];
	    float3 absNormal   = abs3(normal);
	    float  nDotC       = dot3( normal, planeToPoint );
	    if( nDotC > dot3( abs3(normal), absHalf ) )
	    {
	        return false;
	    }
	}
}

bool TooSmall() {

}

void main() {
	
}