struct vector2
{
    float x;
    float y;
    vector2 normalized()
    {
        return { (float)(x > 0) - (x < 0), (float)(y > 0) - (y < 0) };
    }
};