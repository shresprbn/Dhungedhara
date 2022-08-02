
rgba GetColor(float lum)
{
    rgba gs;
    int pixel_bw = (int)(13.0f * lum);
    switch (pixel_bw)
    {
    case 0:gs.r = 0; gs.g = 0; gs.b = 0; gs.a = 255; break;

    case 1:gs.r = 25; gs.g = 25; gs.b = 10; gs.a = 255; break;
    case 2:gs.r = 50; gs.g = 50; gs.b = 70; gs.a = 255; break;
    case 3:gs.r = 75; gs.g = 75; gs.b = 70; gs.a = 255; break;
    case 4:gs.r = 100; gs.g = 100; gs.b = 70; gs.a = 255; break;

    case 5:gs.r = 125; gs.g = 125; gs.b = 40; gs.a = 255; break;
    case 6:gs.r = 150; gs.g = 150; gs.b = 40; gs.a = 255; break;
    case 7:gs.r = 175; gs.g = 175; gs.b = 40; gs.a = 255; break;
    case 8:gs.r = 200; gs.g = 200; gs.b = 40; gs.a = 255; break;

    case 9:gs.r = 225; gs.g = 225; gs.b =40; gs.a = 255; break;
    case 10:gs.r = 220; gs.g = 160; gs.b = 40; gs.a = 220; break;
    case 11:gs.r = 240; gs.g = 185; gs.b = 40; gs.a = 240; break;
    case 12:gs.r = 255; gs.g = 200; gs.b = 40; gs.a = 255; break;
    default:
        gs.r = 0; gs.g = 0; gs.b = 0; gs.a = 255;
    }

    return gs;
}

rgba GetColorWater(float lum)
{
    rgba gc;
    int pixel_bw = (int)(13.0f * lum);
    switch (pixel_bw)
    {
    case 0:gc.r = 0; gc.g = 0; gc.b = 0; gc.a = 255; break;

    case 1:gc.r = 2; gc.g = 2; gc.b = 5; gc.a = 255; break;
    case 2:gc.r = 4; gc.g = 3; gc.b = 10; gc.a = 255; break;
    case 3:gc.r = 6; gc.g = 5; gc.b = 20; gc.a = 255; break;
    case 4:gc.r = 8; gc.g = 10; gc.b = 30; gc.a = 255; break;

    case 5:gc.r = 10; gc.g = 20; gc.b = 40; gc.a = 255; break;
    case 6:gc.r = 12; gc.g = 30; gc.b = 60; gc.a = 255; break;
    case 7:gc.r = 14; gc.g = 40; gc.b = 90; gc.a = 255; break;
    case 8:gc.r = 16; gc.g = 50; gc.b = 120; gc.a = 255; break;

    case 9:gc.r = 18; gc.g = 60; gc.b =150; gc.a = 255; break;
    case 10:gc.r = 20; gc.g = 70; gc.b = 170; gc.a = 220; break;
    case 11:gc.r = 22; gc.g = 80; gc.b = 200; gc.a = 240; break;
    case 12:gc.r = 24; gc.g = 90; gc.b = 230; gc.a = 255; break;
    default:
        gc.r = 0; gc.g = 0; gc.b = 0; gc.a = 255;
    }

    return gc;
}

rgba GetColorGround(float lum)
{
    rgba gs;
    int pixel_bw = (int)(13.0f * lum);
    switch (pixel_bw)
    {
    case 0:gs.r = 64; gs.g = 200; gs.b = 33; gs.a = 255; break;

    case 1:gs.r = 20; gs.g = 180; gs.b = 1; gs.a = 255; break;
    case 2:gs.r = 24; gs.g = 150; gs.b = 3; gs.a = 255; break;
    case 3:gs.r = 28; gs.g = 120; gs.b = 6; gs.a = 255; break;
    case 4:gs.r = 32; gs.g = 100; gs.b = 9; gs.a = 255; break;

    case 5:gs.r = 36; gs.g = 90; gs.b = 12; gs.a = 255; break;
    case 6:gs.r = 40; gs.g = 85; gs.b = 15; gs.a = 255; break;
    case 7:gs.r = 44; gs.g = 80; gs.b = 18; gs.a = 255; break;
    case 8:gs.r = 48; gs.g = 70; gs.b = 21; gs.a = 255; break;

    case 9:gs.r = 52; gs.g = 76; gs.b =24; gs.a = 255; break;
    case 10:gs.r = 56; gs.g = 83; gs.b = 27; gs.a = 220; break;
    case 11:gs.r = 60; gs.g = 90; gs.b = 30; gs.a = 240; break;
    case 12:gs.r = 64; gs.g = 97; gs.b = 33; gs.a = 255; break;
    default:
        gs.r = 0; gs.g = 0; gs.b = 0; gs.a = 255;
    }

    return gs;
}