#include "Model.h"

// From http://www.opengl.org/registry/specs/EXT/pixel_buffer_object.txt 
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Model::Model(Shader* shader) {
    this->shader = shader;
    glGenVertexArrays(1, &vao);                            // Generate a VAO
    glGenBuffers(1, &vbo);                                // Generate a buffer object
    index_vbo = 0;                                        // Will be generated if indices are loaded
    vertices = normals = uvs = colors = NULL;            // Assume no vertex information
    total_vertices = total_normals = total_UVs = total_colors = 0;    // Set all sizes to 0
    vPosition = vNormal = vTexCoord = vColor = umM = -1;
    // By default, the Model matrix is an identity matrix (model will be at the origin)
    mM = glm::mat4(1.0f);

    indices = NULL;
    total_indices = 0;

    umM = glGetUniformLocation(shader->program_ID, "mM");// Find the mM variable in the shader 
    printf ("mM variable in shader is %d\n", umM);
}
Model::~Model() {
    glDisableVertexAttribArray(vPosition);
    glDisableVertexAttribArray(vNormal);
    glDisableVertexAttribArray(vTexCoord);
    glDisableVertexAttribArray(vColor);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    delete (shader);
    delete (vertices);
    delete (normals);
    delete (uvs);
    delete (colors);
}

//Begin data upload functions
void Model::set_geometry(GLfloat* vertices, int size){
    printf ("Copying %d vertices\n", size/sizeof(GLfloat)/3);
    this->vertices = vertices;
    this->total_vertices = size/sizeof(GLfloat)/3;
    vPosition = glGetAttribLocation(shader->program_ID, "vPosition");
    printf ("vPosition ID is: %d\n", vPosition);
}
void Model::set_normals(GLfloat* normals, int size){
    printf ("Copying %d normals....\n", size/sizeof(GLfloat)/3);
    this->normals = normals;
    this->total_normals = size/sizeof(GLfloat)/3;
    vNormal = glGetAttribLocation(shader->program_ID, "vNormal");
    printf ("vNormal ID is: %d\n", vNormal);
}
void Model::set_vertex_colors(GLfloat* colors, int size){
    printf ("Copying %d colors...\n", size/sizeof(GLfloat)/4);
    this->colors = colors;
    this->total_colors = size/sizeof(GLfloat)/4;
    vColor = glGetAttribLocation(shader->program_ID, "vColor");
    printf ("vColor ID is: %d\n", vColor);
}
void Model::set_texture_coordinates(GLfloat* uvs, int size){
    printf ("Copying uvs...\n");
    this->uvs = uvs;
    this->total_UVs = size/sizeof(GLfloat)/2;
    vTexCoord = glGetAttribLocation(shader->program_ID, "vTexCoord");
    printf ("vTexCoord ID is: %d\n", vTexCoord);
}

void Model::set_index_buffer(GLuint* indices, int size){                    // UPDATE!
    printf ("Copying indices...\n");
    this->indices = indices;
    this->total_indices = size/sizeof(GLuint);
    glBindVertexArray(vao);
    glGenBuffers(1, &index_vbo);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, index_vbo);
    // This time, we'll just go ahead and upload the data to the buffer because
    // we're not packing multiple data sets into the buffer
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
    printf ("Loaded %d indices\n", this->total_indices);
}
//End data upload functions



// Pre-condition: You must have called at least one of the functions above
// This function loads all of the data onto the GPU.
void Model::upload_2_server() {
    // Bind the vao
    glBindVertexArray(vao);
    // Bind vbo as the current VBO. Note: the VBO was created in the constructor
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    int offset = 0;
    
    printf ("Loading data onto the GPU...\n");
    // Calculate the size of the buffer we need
    int sizeBuffer = (total_vertices*3+total_normals*3+total_UVs*2+total_colors*4)*sizeof(GLfloat);
    
    // Tell OpenGL that this data probably won't change and is used for drawing (GL_STATIC_DRAW).
    // Do not upload any data yet (NULL), but we specify the size (sizeBuffer). We'll pass data 
    // later using glBufferSubData
    
    // Call glBufferData and tell the GPU how big the buffer is.  Do NOT load any data yet.
    glBufferData(GL_ARRAY_BUFFER, sizeBuffer, NULL, GL_STATIC_DRAW);

    // If the vertices aren't NULL, load them onto the GPU. Offset is currently 0.
    if (vertices) {
        printf ("Loading vertices onto GPU...\n");
        // Load the vertices into the GPU buffer. Note the size is the number of vertices * 3(x, y, z)
        glBufferSubData(GL_ARRAY_BUFFER, offset, total_vertices*3*sizeof(GLfloat), this->vertices);
        // You should read this as "vPosition should read in 3 GL_FLOATs that aren't normalized (GL_FALSE).
        // The data has a stride of 0 (non-interlaced) and starts with an offset of 0 in the buffer."
        glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
        offset+=total_vertices*3*sizeof(GLfloat);
    }
    // Load in the vertex normals right after the vertex coordinates.  Remember, there are 3 values for a normal
    if (normals) {
        printf ("Loading normals onto GPU...\n");
        
        // Load in the vertex data onto the GPU using glBufferSub. Put it right after the vertices
        glBufferSubData(GL_ARRAY_BUFFER, offset, total_normals*3*sizeof(GLfloat), this->normals);
        // Note that the normal data has an offset that starts after the vertex data in the buffer
        glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(offset));
        offset+=total_normals*3*sizeof(GLfloat);
        
    }
    // Load in the texture coordinates right after the normals. Remember, there are 2 values for each UV
    if (uvs) {
        printf ("Loading UVs onto GPU...\n");
        
        // Call glBufferSubData to put the UVs onto the GPU. There are only 2 components in each UV
        glBufferSubData(GL_ARRAY_BUFFER, offset, total_UVs*2*sizeof(GLfloat), this->uvs);
        // Call glVertexAttribPointer to tell vTexCoord where to find it's data. 
        glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(offset));
        offset+=total_UVs*2*sizeof(GLfloat);
    }
    // Load in the color coordinates right after the texture coordinates. There are 4 values for a color
    if (colors) {
        printf ("Loading colors onto GPU...\n");
        
        // Put the color info onto the GPU. CAREFUL! There are 4 parts to each color
        glBufferSubData(GL_ARRAY_BUFFER, offset, total_colors*4*sizeof(GLfloat), this->colors);
        // Specify where the color attributes (vColor) are located
        glVertexAttribPointer (vColor, 4, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(offset));
        offset+=total_colors*4*sizeof(GLfloat);
    }
}

void Model::render(){
    glBindVertexArray(vao);                        // Activate the VAO again. All that buffer info is remembered
    glUseProgram(shader->program_ID);            // Use the shader program
    if (vertices) {
        glEnableVertexAttribArray(vPosition);    // It's a pain, but we always have to enable shader variables
    }
    if (normals) {
        glEnableVertexAttribArray(vNormal);
    }
    if (vTexCoord != -1) {
        glEnableVertexAttribArray(vTexCoord);
    }
    if (vColor != -1) {
        glEnableVertexAttribArray(vColor);
    }
    if (umM != -1) {
        // Upload Model matrix to the uniform
        //glUniformMatrix4fv( umM, 1, GL_FALSE, &mM[0][0] );
        glUniformMatrix4fv( umM, 1, GL_FALSE, glm::value_ptr( mM ) );
    }

    // UPDATE!
    // Now, we can draw two different ways - depending on whether or not the index buffer
    // is active. If indices is NULL, then draw triangles. Otherwise, draw elements
    if (!indices) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glDrawArrays(GL_TRIANGLES, 0, total_vertices);
    }else {
        // 1) What kind of elements we're drawing
        // 2) The number of indices we're drawing (can be less than total_indices)
        // 3) The data type of the indices
        // 4) An offset within the buffer of where we should start drawing
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);
        glDrawElements(GL_TRIANGLES, total_indices, GL_UNSIGNED_INT, NULL);
    }

    if (vertices) {
        glDisableVertexAttribArray(vPosition);    // Disable the variables
    }
    if (normals) {
        glDisableVertexAttribArray(vNormal);
    }
    //if (vTexCoord) {
    if (uvs) {
        glDisableVertexAttribArray(vTexCoord);
    }
    //if (vColor) {
    if (colors) {
        glDisableVertexAttribArray(vColor);
    }
}