
void main ()  
{   
    gl_FragColor = vec4(vec3(length(gl_FragCoord.xy)/1000.0), 1.0);
}
