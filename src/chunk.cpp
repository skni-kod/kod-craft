

void Chunk::draw() {
    int chunkSize = this->parent->getChunkSize();

    for (int x = 0; x < chunkSize; x++)
    for (int y = 0; y < chunkSize; y++)
    for (int z = 0; z < chunkSize; z++) {
        this->blocks[x][y][z].draw();
    }
}
