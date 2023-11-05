#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
// uniform vec2 mousePos;

// Output fragment color
out vec4 finalColor;


//struct MaterialProperty {
//    vec3 color;
//    int useSampler;
//    sampler2D sampler;
//};

void main()
{
    // vec2 myMouse = 2 * (mousePos - 0.5);
    vec2 myCoords = 2 * (fragTexCoord - 0.5); 
    vec4 texelColor0 = texture(texture0, fragTexCoord);
    // test comment asdasdqwd
    float dist = (1 - 0.1 * distance(vec2(0, 0), myCoords)); 
    float ss = smoothstep(0.50, 0.75, dist);
    //texelColor0 *= dist;
    finalColor = 0.5 * texelColor0;
}