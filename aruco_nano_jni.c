#include <opencv4/opencv2/opencv.hpp>
#include "aruco_nano.h"

extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_example_JniInterface_detect(JNIEnv *env, jobject thiz, jlong mat_addr,
                                            jint max_attempts_per_candidate, jint dict) {
    cv::Mat &img = *(cv::Mat *) mat_addr;

    std::vector<Marker> markers = Marker::detect(img, max_attempts_per_candidate,
                                         static_cast<Marker::Dict>(dict));

    jclass markerClass = env->FindClass("ArucoNanoJNI$Marker");
    if (markerClass == NULL) {
        return NULL; // exception thrown
    }

    jmethodID markerConstructor = env->GetMethodID(markerClass, "<init>", "()V");
    if (markerConstructor == NULL) {
        return NULL; // exception thrown
    }

    jobjectArray returnArray = env->NewObjectArray(markers.size(), markerClass, NULL);

    // Fill the return array
    for (size_t i = 0; i < markers.size(); i++) {
        jobject markerObject = env->NewObject(markerClass, markerConstructor);

        // Set the id field
        jfieldID idField = env->GetFieldID(markerClass, "id", "I");
        env->SetIntField(markerObject, idField, markers[i].id);

        jfieldID pointsField = env->GetFieldID(markerClass, "points", "[[F");
        jobjectArray pointsArray = env->NewObjectArray(4, env->FindClass("[F"), NULL);
        for (int j = 0; j < 4; j++) {
            jfloatArray pointArray = env->NewFloatArray(2);
            jfloat point[2] = {markers[i][j].x, markers[i][j].y};
            env->SetFloatArrayRegion(pointArray, 0, 2, point);
            env->SetObjectArrayElement(pointsArray, j, pointArray);
            env->DeleteLocalRef(pointArray);
        }
        env->SetObjectField(markerObject, pointsField, pointsArray);
        env->DeleteLocalRef(pointsArray);

        // Add the Marker object to the return array
        env->SetObjectArrayElement(returnArray, i, markerObject);
        env->DeleteLocalRef(markerObject);
    }

    return returnArray;
}
