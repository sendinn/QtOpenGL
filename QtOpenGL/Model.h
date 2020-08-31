#pragma once
#include "Mesh.h"
#include "../packages/Assimp.3.0.0/build/native/include/assimp/Importer.hpp"
#include "../packages/Assimp.3.0.0/build/native/include/assimp/postprocess.h"
#include "../packages/Assimp.3.0.0/build/native/include/assimp/scene.h"

class Model
{
public:
	Model(QString const& path, QOpenGLShaderProgram* shader);
	~Model();

	Mesh* processMesh(aiMesh *mesh, const aiScene *scene);
	void processNode(aiNode *node, const aiScene *scene);
	void LoadModel(QString const& path);

	QVector<Mesh*> GetMeshes() {
		return m_Meshes;
	}

	QVector<ModelVertex>	m_Vertices;


	QOpenGLShaderProgram* m_ProgramShader;
	QVector<Mesh*>	m_Meshes;
	QString m_Directory;
};

