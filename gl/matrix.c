#include "depend.h"

static 
char key_action = 0,
	 key_shift	= 0,
	 key_up 	= 0,
	 key_down 	= 0,
	 key_left 	= 0,
	 key_right 	= 0,
	 key_w		= 0,
	 key_a		= 0,
	 key_s		= 0,
	 key_d		= 0;

void camera_key_input (int key, int action) {
	
	key_action = 1;
	switch (key) {
		case GLFW_KEY_LEFT_SHIFT:
			key_shift = action != GLFW_RELEASE;
			break;
		
		case GLFW_KEY_UP: 
			key_up = action != GLFW_RELEASE;
			break;
			
		case GLFW_KEY_DOWN: 
			key_down = action != GLFW_RELEASE;
			break;
			
		case GLFW_KEY_LEFT:
			key_left = action != GLFW_RELEASE;
			break;
			
		case GLFW_KEY_RIGHT:
			key_right = action != GLFW_RELEASE;
			break;
			
		case GLFW_KEY_W:
			key_w = action != GLFW_RELEASE;
			break;
			
		case GLFW_KEY_S:
			key_s = action != GLFW_RELEASE;
			break;
			
		case GLFW_KEY_A:
			key_a = action != GLFW_RELEASE;
			break;
			
		case GLFW_KEY_D:
			key_d = action != GLFW_RELEASE;
			break;
	}
	
	if (!key_up 	&&
		!key_down 	&&
		!key_left 	&&
		!key_right 	&&
		!key_w		&&
		!key_a		&&
		!key_s		&&
		!key_d		)
		key_action = 0;
}

void camera_matrix (mat4 camera, float deltaTime) {
	
	glm_mat4_copy (GLM_MAT4_IDENTITY, camera);
	
	static vec3
		cameraPos   = {0.0f, 0.0f,  3.0f},
		cameraFront = {0.0f, 0.0f, -1.0f},
		cameraUp 	= {0.0f, 1.0f,  0.0f};

	static float
		yaw = 90.0f, pitch = 0.0f, 
		cameraSpeed = 6.0f, moveSpeed = 0.4f;
	
	if (key_shift) {
		cameraSpeed *= 0.25f;
		moveSpeed *= 0.25f;
	}
	
	if (key_action) {
		
		if (key_w || key_s) {
			vec3 v;
			glm_vec3_copy  (cameraFront, v);
			(key_w) ?
				glm_vec3_scale (v, deltaTime * moveSpeed, v)
				: glm_vec3_scale (v, -deltaTime * moveSpeed, v);
			v[1] = 0;
			glm_vec3_add (cameraPos, v, cameraPos);
		}
		
		if (key_a || key_d) {
			vec3 v;
			glm_cross (cameraFront, cameraUp, v);
			glm_normalize (v);
			(key_a) ?
				glm_vec3_scale (v, -deltaTime * moveSpeed, v)
				: glm_vec3_scale (v, deltaTime * moveSpeed, v);
			v[1] = 0;
			glm_vec3_add (cameraPos, v, cameraPos);
		}
		
		if (key_up || key_down)
			pitch += (key_up) ?
				cameraSpeed * deltaTime
				: -cameraSpeed * deltaTime;
				
		if (key_left || key_right)
			yaw += (key_left) ?
				cameraSpeed * deltaTime
				: -cameraSpeed * deltaTime;
		
		pitch += 90.0f;
		if (pitch > 175.0f) pitch = 175.0f;
		if (pitch < 5.0f)	pitch = 5.0f;
		pitch -= 90.0f;
		
		if (yaw > 360.0f) yaw -= 360.0f;
		if (yaw < 0.0f)   yaw += 360.0f;
		
		cameraFront[2] = -cos (glm_rad (pitch));
		cameraFront[1] =  sin (glm_rad (pitch));
		cameraFront[0] =  cos (glm_rad (yaw));
		cameraFront[2] *= sin (glm_rad (yaw));
	}
	
	glm_vec3_add (cameraFront, cameraPos, cameraFront);
	glm_lookat	 (cameraPos, cameraFront, cameraUp, camera);
	
	// Restore data
	glm_vec3_sub (cameraFront, cameraPos, cameraFront);
	if (key_shift) {
		cameraSpeed *= 4.0f;
		moveSpeed *= 4.0f;
	}
	
}

