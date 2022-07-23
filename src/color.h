
rgba GetColor(float lum)
{
	rgba gs;
	int pixel_bw = (int)(13.0f * lum);
	switch (pixel_bw)
	{
	case 0:gs.r = 0; gs.g = 0; gs.b = 0; gs.a = 255; break;

	case 1:gs.r = 25; gs.g = 25; gs.b = 25; gs.a = 255; break;
	case 2:gs.r = 50; gs.g = 50; gs.b = 50; gs.a = 255; break;
	case 3:gs.r = 75; gs.g = 75; gs.b = 75; gs.a = 255; break;
	case 4:gs.r = 100; gs.g = 100; gs.b = 100; gs.a = 255; break;

	case 5:gs.r = 125; gs.g = 125; gs.b = 125; gs.a = 255; break;
	case 6:gs.r = 150; gs.g = 150; gs.b = 150; gs.a = 255; break;
	case 7:gs.r = 175; gs.g = 175; gs.b = 175; gs.a = 255; break;
	case 8:gs.r = 200; gs.g = 200; gs.b = 200; gs.a = 255; break;

	case 9:gs.r = 225; gs.g = 225; gs.b = 225; gs.a = 255; break;
	case 10:gs.r = 250; gs.g = 250; gs.b = 250; gs.a = 220; break;
	case 11:gs.r = 255; gs.g = 255; gs.b = 255; gs.a = 240; break;
	case 12:gs.r = 255; gs.g = 255; gs.b = 255; gs.a = 255; break;
	default:
		gs.r = 0; gs.g = 0; gs.b = 0; gs.a = 255;
	}

	return gs;
}