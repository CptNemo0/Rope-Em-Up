#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace llr
{
	const glm::vec3 kRight   = glm::vec3(1.0f, 0.0f, 0.0f);
	const glm::vec3 kUp      = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 kForward = glm::vec3(0.0f, 0.0f, -1.0f);

	class Camera
	{
	private:
		
		glm::vec3 position_;
		
		glm::vec3 right_;
		glm::vec3 up_;
		glm::vec3 front_;

		float pitch_;
		float yaw_;

		float fov_;
		float near_;
		float far_;
		float aspect_ratio_;

	public:

		Camera();
		Camera(glm::vec3 position, float pitch, float yaw, float fov, float near, float far, float ar) :
		position_(position),
		right_(kRight),
		up_(kUp),
		front_(kForward),
		pitch_(pitch),
		yaw_(yaw),
		fov_(fov),
		near_(near),
		far_(far),
		aspect_ratio_(ar){}

		~Camera() = default;

		inline void set_position(glm::vec3 new_position)
		{
			this->position_ = new_position;
		}

		inline glm::vec3 get_position()
		{
			return this->position_;
		}


		inline void set_right(glm::vec3 new_right)
		{
			this->right_ = new_right;
		}

		inline glm::vec3 get_right()
		{
			return this->right_;
		}

		inline void set_up(glm::vec3 new_up)
		{
			this->up_ = new_up;
		}

		inline glm::vec3 get_up()
		{
			return this->up_;
		}

		inline void set_front(glm::vec3 new_front)
		{
			this->front_ = new_front;
		}

		inline glm::vec3 get_front()
		{
			return this->front_;
		}

		
		inline void set_pitch(float new_pitch)
		{
			this->pitch_ = new_pitch;
		}

		inline float get_pitch()
		{
			return pitch_;
		}

		inline void set_yaw(float new_yaw)
		{
			this->yaw_ = new_yaw;
		}

		inline float get_yaw()
		{
			return yaw_;
		}


		inline void set_fov(float new_fov)
		{
			this->fov_ = new_fov;
		}

		inline float get_fov()
		{
			return fov_;
		}

		inline void set_near(float new_near)
		{
			this->near_ = new_near;
		}

		inline float get_near()
		{
			return near_;
		}

		inline void set_far(float new_far)
		{
			this->far_ = new_far;
		}

		inline float get_far()
		{
			return far_;
		}

		inline void set_aspect_ratio(float new_aspect_ratio)
		{
			this->aspect_ratio_ = new_aspect_ratio;
		}

		inline float get_aspect_ratio()
		{
			return aspect_ratio_;
		}


		inline glm::mat4 GetViewMatrix()
		{
			return glm::lookAt(position_, position_ + front_, up_);
		}
	
		void UpdateDirectionVectors();
	};
}



#endif